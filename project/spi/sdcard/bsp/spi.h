#ifndef BSP_SPI_H_
#define BSP_SPI_H_

#include "stm32f0xx.h"

void spi_init(void);
void spi_set_speed(uint8_t speed);
uint8_t spi_wr_byte(uint8_t data);

#endif /* End of BSP_SPI_H_ */
