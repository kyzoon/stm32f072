#include "spi.h"

#define SPI_PORT							SPI1
#define SPI_PORT_CLK						RCC_APB2Periph_SPI1

#define SPI_SCK_GPIO_PIN					GPIO_Pin_5
#define SPI_SCK_GPIO_PORT					GPIOA
#define SPI_SCK_GPIO_CLK					RCC_AHBPeriph_GPIOA
#define SPI_SCK_PINSRC						GPIO_PinSource5
#define SPI_SCK_AF							GPIO_AF_0

#define SPI_MISO_GPIO_PIN					GPIO_Pin_6
#define SPI_MISO_GPIO_PORT					GPIOA
#define SPI_MISO_GPIO_CLK					RCC_AHBPeriph_GPIOA
#define SPI_MISO_PINSRC						GPIO_PinSource6
#define SPI_MISO_AF							GPIO_AF_0

#define SPI_MOSI_GPIO_PIN					GPIO_Pin_7
#define SPI_MOSI_GPIO_PORT					GPIOA
#define SPI_MOSI_GPIO_CLK					RCC_AHBPeriph_GPIOA
#define SPI_MOSI_PINSRC						GPIO_PinSource7
#define SPI_MOSI_AF							GPIO_AF_0

SPI_InitTypeDef s_spi_init_t;

void spi_init(void)
{
	GPIO_InitTypeDef gpio_init_t = {0};

	RCC_APB2PeriphClockCmd(SPI_PORT_CLK, ENABLE);
	RCC_AHBPeriphClockCmd(SPI_MOSI_GPIO_CLK
						| SPI_MISO_GPIO_CLK
						| SPI_SCK_GPIO_CLK, ENABLE);

	GPIO_PinAFConfig(SPI_SCK_GPIO_PORT, SPI_SCK_PINSRC, SPI_SCK_AF);
	GPIO_PinAFConfig(SPI_MISO_GPIO_PORT, SPI_MISO_PINSRC, SPI_MISO_AF);
	GPIO_PinAFConfig(SPI_MOSI_GPIO_PORT, SPI_MOSI_PINSRC, SPI_MOSI_AF);

	gpio_init_t.GPIO_Pin = SPI_SCK_GPIO_PIN;
	gpio_init_t.GPIO_Mode = GPIO_Mode_AF;
	gpio_init_t.GPIO_Speed = GPIO_Speed_Level_3;
	gpio_init_t.GPIO_OType = GPIO_OType_PP;
	gpio_init_t.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(SPI_SCK_GPIO_PORT, &gpio_init_t);

	gpio_init_t.GPIO_Pin = SPI_MISO_GPIO_PIN;
	GPIO_Init(SPI_MISO_GPIO_PORT, &gpio_init_t);

	gpio_init_t.GPIO_Pin = SPI_MOSI_GPIO_PIN;
	GPIO_Init(SPI_MOSI_GPIO_PORT, &gpio_init_t);

	s_spi_init_t.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
	s_spi_init_t.SPI_Mode = SPI_Mode_Master;
	s_spi_init_t.SPI_DataSize = SPI_DataSize_8b;
	s_spi_init_t.SPI_CPOL = SPI_CPOL_High;
	s_spi_init_t.SPI_CPHA = SPI_CPHA_2Edge;
	s_spi_init_t.SPI_NSS = SPI_NSS_Soft;
	s_spi_init_t.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_256;
	s_spi_init_t.SPI_FirstBit = SPI_FirstBit_MSB;
	s_spi_init_t.SPI_CRCPolynomial = 7;
	SPI_Init(SPI_PORT, &s_spi_init_t);

	SPI_RxFIFOThresholdConfig(SPI_PORT, SPI_RxFIFOThreshold_QF);
	SPI_Cmd(SPI_PORT, ENABLE);

	spi_wr_byte(0xFF);
}

void spi_set_speed(uint8_t speed)
{
	s_spi_init_t.SPI_BaudRatePrescaler = speed;
	SPI_Init(SPI_PORT, &s_spi_init_t);
	SPI_Cmd(SPI_PORT, ENABLE);
}

uint8_t spi_wr_byte(uint8_t data)
{
	uint8_t retry = 0;

	while(RESET == SPI_I2S_GetFlagStatus(SPI_PORT, SPI_I2S_FLAG_TXE))
	{
		++retry;
		if(200 < retry)
		{
			return 0;
		}
	}

	SPI_SendData8(SPI_PORT, data);
	retry = 0;
	while(RESET == SPI_I2S_GetFlagStatus(SPI_PORT, SPI_I2S_FLAG_RXNE))
	{
		++retry;
		if(200 < retry)
		{
			return 0;
		}
	}

	return SPI_ReceiveData8(SPI_PORT);
}
