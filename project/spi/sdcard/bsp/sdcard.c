#include <stdio.h>

#include "sdcard.h"

#include "spi.h"
#include "usart.h"

// SD 卡部分指令集
#define CMD0						(0)		// 卡复位
#define CMD1						(1)
#define CMD8						(8)		// SEND_IF_COND
#define CMD9						(9)		// 读取 CSD
#define CMD10						(10)	// 读取 CID
#define CMD12						(12)	// 停止数据传输
#define CMD16						(16)	// 设置扇区大小，返回 0x00
#define CMD17						(17)	// 读扇区
#define CMD18						(18)	// 读多个扇区
#define CMD23						(23)	// 写多扇区写入前，预先擦除 N 个扇区
#define CMD24						(24)	// 写扇区
#define CMD25						(25)	// 写多个扇区
#define CMD41						(41)	// 返回 0x00
#define CMD55						(55)	// 返回 0x01
#define CMD58						(58)	// 读取 OCR 信息
#define CMD59						(59)	// 使能/禁止 CRC，返回 0x00

// 数据写入回应
#define MSD_DATA_OK					(0x05)
#define MSD_DATA_CRC_ERROR			(0x0B)
#define MSD_DATA_WRITE_ERROR		(0x0D)
#define MSD_DATA_OTHER_ERROR		(0xFF)
// SD 卡回应标志 r1 位
#define MSD_RESP_OK					(0x00)
#define MSD_IN_IDLE_STATE			(0x01)
#define MSD_ERASE_RESET				(0x02)
#define MSD_ILLEGAL_COMMAND			(0x04)
#define MSD_COM_CRC_ERROR			(0x08)
#define MSD_ERASE_SEQUENCE_ERROR	(0x10)
#define MSD_ADDRESS_ERROR			(0x20)
#define MSD_PARAMETER_ERROR			(0x40)
#define MSD_RESP_FAILUER			(0xFF)

// SD 卡片选控制 GPIO
#define SD_CS_GPIO_PIN				GPIO_Pin_13
#define SD_CS_GPIO_PORT				GPIOB
#define SD_CS_GPIO_CLK				RCC_AHBPeriph_GPIOB


uint8_t g_sd_type = 0;

static void sd_spi_set_speed_low(void)
{
	spi_set_speed(SPI_BaudRatePrescaler_256);
}

static void sd_spi_set_speed_high(void)
{
	spi_set_speed(SPI_BaudRatePrescaler_2);
}

static void sd_spi_init(void)
{
	GPIO_InitTypeDef gpio_init_t;

	RCC_AHBPeriphClockCmd(SD_CS_GPIO_CLK, ENABLE);

	gpio_init_t.GPIO_Pin = SD_CS_GPIO_PIN;
	gpio_init_t.GPIO_Mode = GPIO_Mode_OUT;
	gpio_init_t.GPIO_Speed = GPIO_Speed_50MHz;
	gpio_init_t.GPIO_OType = GPIO_OType_PP;
	gpio_init_t.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(SD_CS_GPIO_PORT, &gpio_init_t);

	GPIO_SetBits(SD_CS_GPIO_PORT, SD_CS_GPIO_PIN);

	spi_init();

	GPIO_SetBits(SD_CS_GPIO_PORT, SD_CS_GPIO_PIN);
}

static void sd_disselect(void)
{
	GPIO_SetBits(SD_CS_GPIO_PORT, SD_CS_GPIO_PIN);
	spi_wr_byte(0xFF);
}

// Return: 0 - OK, 1 - Failed
static uint8_t sd_select(void)
{
	GPIO_ResetBits(SD_CS_GPIO_PORT, SD_CS_GPIO_PIN);
	if(0 == sd_wait_ready())
	{
		return 0;
	}

	sd_disselect();

	return 1;
}

// Return: 0 - OK, Other - Failed
static uint8_t sd_recv_data(uint8_t *buf, uint16_t len)
{
	// 等待 SD 卡回应：数据起始令牌 0xFE
	if(0 != sd_get_resp(0xFE))
	{
		return 1;
	}

	while(len--)
	{
		*buf = spi_wr_byte(0xFF);
		++buf;
	}

	// 读取两个伪 CRC (dummy CRC)
	spi_wr_byte(0xFF);
	spi_wr_byte(0xFF);

	return 0;
}


// Return: 0 - OK, other - Failed
static uint8_t sd_send_block(uint8_t *buf, uint8_t cmd)
{
	uint32_t i = 0;
	uint8_t ret = 0;

	if(0 == sd_wait_ready())
	{
		return 1;
	}

	spi_wr_byte(cmd);
	if(0xFD != cmd)
	{
		for(i = 0; i < 512; ++i)
		{
			spi_wr_byte(buf[i]);
		}
		// 提高速度，减少函数传参时间
		// 读取两个伪 CRC (dummy CRC)，并忽略
		spi_wr_byte(0xFF);
		spi_wr_byte(0xFF);
		// 读取响应
		ret = spi_wr_byte(0xFF);
		if(0x05 != (0x1F & ret))
		{
			return 2;
		}
	}
	
	return 0;
}

static uint8_t sd_send_cmd(uint8_t cmd, uint32_t arg, uint8_t crc)
{
	uint8_t r1 = 0;
	uint32_t retry = 0x1F;

	sd_disselect();
	if(0 != sd_select())
	{
		return 0xFF;
	}

	spi_wr_byte(cmd | 0x40);
	spi_wr_byte(arg >> 24);
	spi_wr_byte(arg >> 16);
	spi_wr_byte(arg >> 8);
	spi_wr_byte(arg);
	spi_wr_byte(crc);

	// CMD12 停止数据传输时，忽略一个字节数据
	if(CMD12 == cmd)
	{
		spi_wr_byte(0xFF);
	}

	do
	{
		r1 = spi_wr_byte(0xFF);
	}
	while((r1 & 0x80) && retry--);

	return r1;
}

////////////////////////////////////////////////////////////////////////////////
// Public Functions
////////////////////////////////////////////////////////////////////////////////
// Return: 0 - OK, 1 - Failed
uint8_t sd_wait_ready(void)
{
	uint32_t retry = 0;

	do
	{
		if(0xFF == spi_wr_byte(0xFF))
		{
			return 0;
		}
		++retry;
	}
	while(0xFFFFFF > retry);
	
	return 1;
}

// Return: 0 - OK, Other - Failed
uint8_t sd_get_resp(uint8_t resp)
{
	uint32_t retry = 0xFFF;

	while((resp != spi_wr_byte(0xFF)) && retry--);

	if(0 == retry)
	{
		return MSD_RESP_FAILUER;
	}

	return MSD_RESP_OK;
}

// uint8_t *cid - 至少 16 字节缓存区
// Return: 0 - OK, 1 - Failed
uint8_t sd_get_cid(uint8_t *cid)
{
	uint8_t r1 = 0;

	r1 = sd_send_cmd(CMD10, 0, 0x01);
	if(0x00 == r1)
	{
		r1 = sd_recv_data(cid, 16);
	}

	sd_disselect();

	if(0x00 != r1)
	{
		return 1;
	}

	return 0;
}

// uint8_t *csd - 至少 16 字节缓存区
// Return: 0 - OK, 1 - Failed
uint8_t sd_get_csd(uint8_t *csd)
{
	uint8_t r1 = 0;

	r1 = sd_send_cmd(CMD9, 0, 0x01);	// 0x49
	if(0x00 == r1)
	{
		r1 = sd_recv_data(csd, 16);
	}

	sd_disselect();

	if(0x00 != r1)
	{
		return 1;
	}

	return 0;
}

// Return: 0 - Error, other - sector count
uint32_t sd_get_sector_cnt(void)
{
	uint8_t csd[16] = {0};
	uint32_t sector_cnt = 0;
	uint8_t n = 0;
	uint16_t csize = 0;

	if(0 != sd_get_csd(csd))
	{
		return 0;
	}

	if(0x40 == (csd[0] & 0xC0))	// V2.00 版本卡
	{
		csize = csd[9] + ((uint16_t)csd[8] << 8) + 1;
		sector_cnt = (uint32_t)csize << 10;
	}
	else	// V1.xx 版本卡
	{
		n = ((csd[10] & 0x80) >> 7) + ((csd[9] & 0x3) << 1) + (csd[5] & 0x0F) + 2;
		csize = (csd[8] >> 6) + ((uint16_t)csd[7] << 2) + ((uint16_t)(csd[6] & 3) << 10) + 1;
		sector_cnt = (uint16_t)csize << (n - 9);
	}

	return sector_cnt;
}

// Return: 0 - ok, 1 - failed
uint8_t sd_idle_state(void)
{
	uint8_t r1 = 0;
	uint32_t i = 0;
	uint32_t retry = 0;

	// 纯延时，等待 SD 卡上电完成
	for(i = 0; i < 0xF00; ++i);
	// 发送最少 74 个脉冲，让 SD 卡自已初始化完成
	for(i = 0; i < 10; ++i)
	{
		spi_wr_byte(0xFF);
	}

	// SD 卡复位到 IDLE 状态
	// 循环连接发送 CMD0，直到 SD 卡返回 0x01，进入 IDLE 状态
	// 超时，则直接退出
	retry = 0;
	do
	{
		r1 = sd_send_cmd(CMD0, 0, 0x95);
		++retry;
	}
	while((MSD_IN_IDLE_STATE != r1) && (200 > retry));

	// 重试超时
	if(200 == retry)
	{
		return 1;
	}

	return 0;
}

// Return: 0 - OK, r1 - SD Response Error Code, 0xAA - other error
uint8_t sd_init(void)
{
	uint8_t r1 = 0;
	uint8_t buf[4] = {0};
	uint32_t retry = 0;
	uint32_t i = 0;

	sd_spi_init();

	sd_spi_set_speed_low();

	// 发送最少 74 个脉冲，让 SD 卡自已初始化完成
	for(i = 0; i < 10; ++i)
	{
		spi_wr_byte(0xFF);
	}

	// 进行 IDLE 状态
	retry = 20;
	do
	{
		r1 = sd_send_cmd(CMD0, 0, 0x95);	// 0x40
	}
	while((r1 != MSD_IN_IDLE_STATE) && retry--);

	g_sd_type = 0;	// 默认无卡
	if(MSD_IN_IDLE_STATE == r1)
	{
		// SD V2.0
		if(1 == sd_send_cmd(CMD8, 0x1AA, 0x87))	// 0x48
		{
			for(i = 0; i < 4; ++i)
			{
				// 获取 r7 返回值的后 4 位
				buf[i] = spi_wr_byte(0xFF);
			}
			// 卡是否支持 2.7 - 3.6V
			if(0x01 == buf[2] && 0xAA == buf[3])
			{
				retry = 0xFFFE;
				do
				{
					sd_send_cmd(CMD55, 0, 0x01);	// 0x77
					r1 = sd_send_cmd(CMD41, 0x40000000, 0x01);	// 0x69
				}
				while(r1 != MSD_RESP_OK && retry--);

				// 鉴别 SD V2.0
				if(retry && 0 == sd_send_cmd(CMD58, 0, 0x01))	// 0x7A
				{
					for(i = 0; i < 4; ++i)
					{
						// 获取 OCR 值
						buf[i] = spi_wr_byte(0xFF);
					}

					if(0x40 & buf[0])
					{
						g_sd_type = SD_TYPE_V2HC;
					}
					else
					{
						g_sd_type = SD_TYPE_V2;
					}
				}
			}
		}
		else	// SD V1.x/MMC V3
		{
			sd_send_cmd(CMD55, 0, 0x01);
			r1 = sd_send_cmd(CMD41, 0, 0x01);
			if(1 >= r1)
			{
				g_sd_type = SD_TYPE_V1;
				retry = 0xFFFE;
				// 等待退出 IDLE 模式
				do
				{
					sd_send_cmd(CMD55, 0, 0x01);
					r1 = sd_send_cmd(CMD41, 0, 0x01);
				}
				while(r1 != MSD_RESP_OK && retry--);
			}
			else
			{
				g_sd_type = SD_TYPE_MMC;	// MMC V3
				retry = 0xFFFE;
				do
				{
					r1 = sd_send_cmd(CMD1, 0, 0x01);
				}
				while(r1 != MSD_RESP_OK && retry--);
			}

			if(0 == retry || MSD_RESP_OK != sd_send_cmd(CMD16, 512, 0x01))
			{
				g_sd_type = SD_TYPE_ERR;
			}
		}
	}

	sd_disselect();
	sd_spi_set_speed_high();

	if(SD_TYPE_ERR != g_sd_type)
	{
		return 0;
	}
	else if(MSD_RESP_OK != r1)
	{
		return r1;
	}

	return 0xAA;
}

// Return: 0 - OK, Other - Failed
uint8_t sd_read_disk(uint8_t *buf, uint32_t sector, uint8_t cnt)
{
	uint8_t r1 = 0;

	// 转换为字节地址
	if(SD_TYPE_V2HC != g_sd_type)
	{
		sector <<= 9;
	}

	if(1 == cnt)
	{
		// 发送读扇区数据命令
		r1 = sd_send_cmd(CMD17, sector, 0x01);
		if(MSD_RESP_OK == r1)
		{
			// 读取 1 个扇区，512 字节
			r1 = sd_recv_data(buf, 512);
		}
	}
	else
	{
		// 发送读多个扇区数据命令
		r1 = sd_send_cmd(CMD18, sector, 0x01);
		do
		{
			// 读取 1 个扇区，512 字节
			r1 = sd_recv_data(buf, 512);
			buf += 512;
		}
		while(--cnt && (MSD_RESP_OK == r1));
		// 发送停止数据传输命令
		sd_send_cmd(CMD12, 0, 0x01);
	}
	
	sd_disselect();

	return r1;
}

// Return: 0 - ok, other - failed
uint8_t sd_write_disk(uint8_t *buf, uint32_t sector, uint8_t cnt)
{
	uint8_t r1 = 0;

	// 转换为字节地址
	if(SD_TYPE_V2HC != g_sd_type)
	{
		sector <<= 9;
	}

	if(1 == cnt)
	{
		// 发送写扇区数据命令
		r1 = sd_send_cmd(CMD24, sector, 0x01);
		if(MSD_RESP_OK == r1)
		{
			// 读取 1 个扇区，512 字节
			r1 = sd_send_block(buf, 0xFE);
		}
	}
	else
	{
		// 非 MMC
		// 写多个扇区数据前，先擦除 N 个扇区
		if(SD_TYPE_MMC != g_sd_type)
		{
			sd_send_cmd(CMD55, 0, 0x01);
			sd_send_cmd(CMD23, cnt, 0x01);
		}
		// 发送写多个扇区数据命令
		r1 = sd_send_cmd(CMD25, sector, 0x01);
		if(MSD_RESP_OK == r1)
		{
			do
			{
				// 写 1 个扇区，512 字节
				r1 = sd_send_block(buf, 0xFC);
				buf += 512;
			}
			while(--cnt && (MSD_RESP_OK == r1));
			// 发送停止数据传输命令
			r1 = sd_send_block(0, 0xFD);
		}
	}
	
	sd_disselect();

	return r1;
}

// Return: 0 - Error, other - capacity
uint32_t sd_get_capacity(void)
{
	uint8_t csd[16] = {0};
	uint8_t r1 = 0;
	uint32_t csize = 0;
	uint32_t capacity = 0;
	uint32_t i = 0;
	uint32_t temp  = 0;

	if(0 != sd_get_csd(csd))
	{
		return 0;
	}

	if(0x40 == (csd[0] & 0xC0))
	{
		// 扇区数
		csize = csd[9] + ((uint16_t)csd[8] << 8) + 1;
		// 容量，单位 M
		capacity = csize * 512;
	}
	else
	{
		// C size
		i = csd[6] & 0x03;
		i <<= 8;
		i += csd[7];
		i <<= 2;
		i += ((csd[8] & 0xC0) >> 6);
		// c size multi
		r1 = csd[9] & 0x03;
		r1 <<= 1;
		r1 += ((csd[10] & 0x80) >> 7);

		// blocknr
		r1 += 2;
		temp = 1;
		while(r1)
		{
			temp *= 2;
			--r1;
		}
		capacity = ((uint32_t)(i + 1)) * ((uint32_t)temp);

		// REAd BL len
		i = csd[5] & 0x0F;
		// block len
		temp = 1;
		while(i)
		{
			temp *= 2;
			--i;
		}
		// the final result
		capacity *= temp;
	}

	return capacity;
}
