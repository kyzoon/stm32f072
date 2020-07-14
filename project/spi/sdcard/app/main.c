/*--------------------------------------------------------------------------------------------------
Copyright 2019 - 2020, xxx Inc.
License     : Internal Use.
Author      : 
Description : 
--------------------------------------------------------------------------------------------------*/

/* include ANSI C .h file */
#include <stdio.h>
/* include Other Lib .h file */
/* include Project .h file */
#include "stm32f0xx.h"
/* include Local .h file */
#include "led.h"
#include "sdcard.h"
#include "spi.h"
#include "systick.h"
#include "usart.h"

/*------------------------------------------------------------------------------------------------*/
//	Defines and Macros
/*------------------------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------------------------*/
//	Types and Enums
/*------------------------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------------------------*/
//	Constants
/*------------------------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------------------------*/
//	Variables
/*------------------------------------------------------------------------------------------------*/
/* Private/Static */
/*
 * Function   : 
 * Value Area : 
 */
uint8_t s_buf[512] = {0};
/* Global */

/*------------------------------------------------------------------------------------------------*/
//	Extern
/*------------------------------------------------------------------------------------------------*/
/* Variables */

/* Function prototypes */

/*------------------------------------------------------------------------------------------------*/
//	Static Functions
/*------------------------------------------------------------------------------------------------*/
#if 1
/*--------------------------------------------------------------------------------------------------
Prototype      : 
Parameters     : 
Return         : 
Implementation : 
--------------------------------------------------------------------------------------------------*/

#endif /* End of 1, Just a tag, ignore it */

/*------------------------------------------------------------------------------------------------*/
//	Public Functions
/*------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------
Prototype      : 
Parameters     : 
Return         : 
Implementation : 
--------------------------------------------------------------------------------------------------*/
int main(void)
{
	uint32_t sector_size = 0;
	uint32_t t = 0;
	uint32_t capacity = 0;

	LED_init();
	systick_init();
	usart1_init();

	printf("\r\n");
	printf("********************\r\n");
	printf("    STM32F072C8T6\r\n");
	printf("********************\r\n");
	
	while(0 != sd_init())
	{
		printf("SD Card Init Failed!\r\n");
		delay_ms(500);
		printf("Please Chekc Hardware!\r\n");

		LED_toggleDS0();
	}

	sector_size = sd_get_capacity();
	printf("SD Sector Size: %08lx\r\n", sector_size);
	printf("SD Version: %d\r\n", g_sd_type);

	capacity = sd_get_capacity();
	printf("SD Sector Size: %08lx\r\n", capacity);
	if(SD_TYPE_V2 == g_sd_type)
	{
		printf("SD Card Version    : V2\r\n");
		printf("SD Card Capacity(M): %f\r\n", (float)capacity / 1024.0 / 1024.0);
		printf("SD Card Capacity(G): %f\r\n", (float)capacity / 1024.0 / 1024 / 1024);
	}
	if(SD_TYPE_V2HC == g_sd_type)
	{
		printf("SD Card Version    : V2HC\r\n");
		printf("SD Card Capacity(M): %f\r\n", (float)capacity / 1024);
		printf("SD Card Capacity(G): %f\r\n", (float)capacity / 1024 / 1024);
	}

	while(1)
	{
		// if(30 == t)
		if(10 == t)
		{
		#if 0
			if(0 == sd_read_disk(s_buf, 0, 1))
			{
				printf("Section 0 Data:\r\n");
				for(sector_size = 0; sector_size < 512; ++sector_size)
				{
					printf("%02x ", s_buf[sector_size]);
				}
				printf("\r\nData Ended\r\n");
			}
		#endif
			t = 0;
		}

		++t;
		delay_ms(1000);
		LED_toggleDS0();
	}
}

#ifdef USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{
	/* User can add his own implementation to report the file name and line number,
	   ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
	
	/* Infinite loop */
	while (1)
	{
	}
}
#endif
