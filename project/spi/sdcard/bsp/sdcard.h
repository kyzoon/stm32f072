#ifndef BSP_SDCARD_H_
#define BSP_SDCARD_H_

#include "stm32f0xx.h"

// SD 卡类型
#define SD_TYPE_ERR					0x00
#define SD_TYPE_MMC					0x01
#define SD_TYPE_V1					0x02
#define SD_TYPE_V2					0x04
#define SD_TYPE_V2HC				0x06

extern uint8_t g_sd_type;

uint8_t sd_init(void);

uint8_t sd_wait_ready(void);

uint8_t sd_get_resp(uint8_t resp);

uint32_t sd_get_sector_cnt(void);

uint8_t sd_get_cid(uint8_t *cid);

uint8_t sd_get_csd(uint8_t *csd);

uint32_t sd_get_capacity(void);

uint8_t sd_read_disk(uint8_t *buf, uint32_t sector, uint8_t cnt);

uint8_t sd_write_disk(uint8_t *buf, uint32_t sector, uint8_t cnt);

#endif  /* End of BSP_SDCARD_H_*/
