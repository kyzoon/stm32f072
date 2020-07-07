/*--------------------------------------------------------------------------------------------------
Copyright 2019 - 2020, xxx Inc.
License     : Internal Use.
Author      : 
Description : 
--------------------------------------------------------------------------------------------------*/

/* include ANSI C .h file */
/* include Other Lib .h file */
/* include Project .h file */
/* include Local .h file */
#include "led.h"

/*------------------------------------------------------------------------------------------------*/
//	Defines and Macros
/*------------------------------------------------------------------------------------------------*/
#define LED0_GPIO_RCC		RCC_AHBPeriph_GPIOC
#define LED0_GPIO_PORT		GPIOC
#define LED0_GPIO_PIN		GPIO_Pin_13

#define LED1_GPIO_RCC		RCC_AHBPeriph_GPIOB
#define LED1_GPIO_PORT		GPIOB
#define LED1_GPIO_PIN		GPIO_Pin_13


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
// 0 - OFF, 1 - ON
static uint8_t s_led0_state = 0;
static uint8_t s_led1_state = 0;

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
Prototype      : void LED_init(void)
Parameters     : None
Return         : None
Implementation : LED初始化
--------------------------------------------------------------------------------------------------*/
void LED_init(void)
{
	GPIO_InitTypeDef gpio_t;

	// 使能PA,PD端口时钟
	RCC_AHBPeriphClockCmd(LED0_GPIO_RCC | LED1_GPIO_RCC, ENABLE);
	
	gpio_t.GPIO_Pin = LED0_GPIO_PIN;
	gpio_t.GPIO_Mode = GPIO_Mode_OUT;
	gpio_t.GPIO_OType = GPIO_OType_PP;
	gpio_t.GPIO_PuPd = GPIO_PuPd_UP;
	gpio_t.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(LED0_GPIO_PORT, &gpio_t);

	GPIO_SetBits(LED0_GPIO_PORT, LED0_GPIO_PIN);

	gpio_t.GPIO_Pin = LED1_GPIO_PIN;
	GPIO_Init(LED1_GPIO_PORT, &gpio_t);
	GPIO_SetBits(LED1_GPIO_PORT, LED1_GPIO_PIN);
}

/*--------------------------------------------------------------------------------------------------
Prototype      : void LED_switchDS0(u8 type)
Parameters     : type - LED_OFF : 关闭
                      - LED_ON  : 打开
Return         : None
Implementation : LED DS0开关功能函数
--------------------------------------------------------------------------------------------------*/
void LED_switchDS0(uint8_t type)
{
	if(LED_OFF == type)
	{
		s_led0_state = 0;
		GPIO_SetBits(LED0_GPIO_PORT, LED0_GPIO_PIN);
	}
	else if(LED_ON == type)
	{
		s_led0_state = 1;
		GPIO_ResetBits(LED0_GPIO_PORT, LED0_GPIO_PIN);
	}
}

/*--------------------------------------------------------------------------------------------------
Prototype      : void LED_toggleDS0(void)
Parameters     : None
Return         : None
Implementation : LED DS0开关切换函数
--------------------------------------------------------------------------------------------------*/
void LED_toggleDS0(void)
{
	if(s_led0_state == 0)
	{
		s_led0_state = 1;
		LED_switchDS0(LED_ON);
	}
	else
	{
		s_led0_state = 0;
		LED_switchDS0(LED_OFF);
	}
}

/*--------------------------------------------------------------------------------------------------
Prototype      : void LED_switchDS1(u8 type)
Parameters     : type - LED_OFF : 关闭
                      - LED_ON  : 打开
Return         : None
Implementation : LED DS0开关功能函数
--------------------------------------------------------------------------------------------------*/
void LED_switchDS1(uint8_t type)
{
	if(LED_OFF == type)
	{
		GPIO_SetBits(LED1_GPIO_PORT, LED1_GPIO_PIN);
	}
	else if(LED_ON == type)
	{
		GPIO_ResetBits(LED1_GPIO_PORT, LED1_GPIO_PIN);
	}
}

/*--------------------------------------------------------------------------------------------------
Prototype      : void LED_toggleDS0(void)
Parameters     : None
Return         : None
Implementation : LED DS0开关切换函数
--------------------------------------------------------------------------------------------------*/
void LED_toggleDS1(void)
{
	if(s_led1_state == 0)
	{
		s_led1_state = 1;
		LED_switchDS1(LED_ON);
	}
	else
	{
		s_led1_state = 0;
		LED_switchDS1(LED_OFF);
	}
}

