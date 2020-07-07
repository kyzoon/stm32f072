#include "usart.h"

#define USART					USART1
#define USART_RCC				RCC_APB2Periph_USART1

#define USART_GPIO_RCC			RCC_AHBPeriph_GPIOA
#define USART_GPIO_PORT			GPIOA
#define USART_TX_GPIO_PIN		GPIO_Pin_9
#define USART_RX_GPIO_PIN		GPIO_Pin_10
#define USART_TX_GPIO_PINSRC	GPIO_PinSource9
#define USART_RX_GPIO_PINSRC	GPIO_PinSource10

void usart1_init(void)
{
	GPIO_InitTypeDef gpio_init_t;
	USART_InitTypeDef usart_init_t;

	RCC_AHBPeriphClockCmd(USART_GPIO_RCC, ENABLE);
	RCC_APB2PeriphClockCmd(USART_RCC, ENABLE);

	GPIO_PinAFConfig(USART_GPIO_PORT, USART_TX_GPIO_PINSRC, GPIO_AF_1);
	GPIO_PinAFConfig(USART_GPIO_PORT, USART_RX_GPIO_PINSRC, GPIO_AF_1);

	gpio_init_t.GPIO_Pin = USART_TX_GPIO_PIN | USART_RX_GPIO_PIN;
	gpio_init_t.GPIO_Mode = GPIO_Mode_AF;
	gpio_init_t.GPIO_OType = GPIO_OType_PP;
	gpio_init_t.GPIO_PuPd = GPIO_PuPd_UP;
	gpio_init_t.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(USART_GPIO_PORT, &gpio_init_t);

	usart_init_t.USART_BaudRate = 9600;
	usart_init_t.USART_WordLength = USART_WordLength_8b;
	usart_init_t.USART_StopBits = USART_StopBits_1;
	usart_init_t.USART_Parity = USART_Parity_No;
	usart_init_t.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	usart_init_t.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(USART, &usart_init_t);

	USART_ClearFlag(USART, USART_FLAG_TC);
	USART_Cmd(USART, ENABLE);
}

int _write(int fd, char *buf, int size)
{
	int i = 0;

	for(i = 0; i < size; ++i)
	{
		while(!(USART1->ISR & USART_ISR_TXE)) {};
		USART_SendData(USART1, buf[i]);
	}

	return size;
}

#if 0
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
	USART_SendData(USART, (uint8_t) ch);
	
	/* Loop until the end of transmission */
	while(USART_GetFlagStatus(USART, USART_FLAG_TC) == RESET) {};
	
	return ch;
}
#endif
