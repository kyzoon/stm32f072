#include <stdio.h>

#include "systick.h"

uint16_t g_tick_counter = 0;
uint32_t g_tick_accumulator = 0;

/**
 * 这个数据类型声明很重要
 */
static __IO uint32_t TimingDelay;

void systick_init(void)
{
    /* SystemCoreClock / 1000000 : 1us 中断 1 次 */
    /* SystemCoreClock / 1000 ：1 ms 中断 1 次 */
    while(SysTick_Config(SystemCoreClock / 1000)) {};
    printf("abc\r\n");
}

void delay_ms(__IO uint32_t ms)
{
    // g_tick_counter = ms;
    TimingDelay = ms;
    // while(0 != g_tick_counter) {
    while(0 != TimingDelay);
        // printf("*** %d\r\n", g_tick_counter);
    // };
}

void TimingDelay_Decrement(void)
{
    if(TimingDelay != 0x00)
    {
        TimingDelay--;
    }
}