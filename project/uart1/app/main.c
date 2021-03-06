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
static void delay(uint32_t millis)
{
	while(millis-- > 0)
	{
		volatile int x = 5971;
		while(x-- > 0){ __asm("nop"); }
	}
}
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
	LED_init();
	usart1_init();

	printf("\r\n");
	printf("********************\r\n");
	printf("Hello STM32F072C8\r\n");
	printf("********************\r\n");

	/* Infinite loop */
	while (1)
	{
		LED_toggleDS0();
		delay(200);
		LED_toggleDS0();
		delay(800);
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
