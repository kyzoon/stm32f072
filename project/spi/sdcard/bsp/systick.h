#ifndef BSP_SYSTICK_H_
#define BSP_SYSTICK_H_

#include "stm32f0xx.h"

void systick_init(void);
void TimingDelay_Decrement(void);

void delay_ms(__IO uint32_t ms);

#endif /* End of BSP_SYSTICK_H_ */