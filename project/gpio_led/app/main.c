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

	/* Infinite loop */
	while (1)
	{
		LED_toggleDS0();
		LED_toggleDS1();
		delay(200);
		LED_toggleDS0();
		LED_toggleDS1();
		delay(200);
	}
}

#ifdef __GNUC__
#define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
#define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif /* __GNUC__ */
/**
  * @brief  Retargets the C library printf function to the USART.
  * @param  None
  * @retval None
  */
PUTCHAR_PROTOTYPE
{
	/* Place your implementation of fputc here */
	/* e.g. write a character to the USART */
	//USART_SendData(EVAL_COM1, (uint8_t) ch);
	
	/* Loop until the end of transmission */
	//while (USART_GetFlagStatus(EVAL_COM1, USART_FLAG_TC) == RESET)
	//{}
	
	return ch;
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
