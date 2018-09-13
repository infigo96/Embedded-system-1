# include "board.h"
# include "compiler.h"
# include "driver.h"
# include "LED.h"


int main (void)
{
	int definePORTs[8] = {	LED0_GPIO / GPIO_MAX_PIN_NUMBER, LED1_GPIO / GPIO_MAX_PIN_NUMBER,
							LED2_GPIO / GPIO_MAX_PIN_NUMBER, LED3_GPIO / GPIO_MAX_PIN_NUMBER,
							LED4_GPIO / GPIO_MAX_PIN_NUMBER, LED5_GPIO / GPIO_MAX_PIN_NUMBER,
							LED6_GPIO / GPIO_MAX_PIN_NUMBER, LED7_GPIO / GPIO_MAX_PIN_NUMBER};
	int definePINs[8] = {	LED0_GPIO & ( GPIO_MAX_PIN_NUMBER -1), LED1_GPIO & ( GPIO_MAX_PIN_NUMBER -1),
							LED2_GPIO & ( GPIO_MAX_PIN_NUMBER -1), LED3_GPIO & ( GPIO_MAX_PIN_NUMBER -1),
							LED4_GPIO & ( GPIO_MAX_PIN_NUMBER -1), LED5_GPIO & ( GPIO_MAX_PIN_NUMBER -1),
							LED6_GPIO & ( GPIO_MAX_PIN_NUMBER -1), LED7_GPIO & ( GPIO_MAX_PIN_NUMBER -1)};
	int defineBITVALUEs[8] = {	1<<definePINs[0], 1<<definePINs[1], 1<<definePINs[2], 1<<definePINs[3],
								1<<definePINs[4], 1<<definePINs[5], 1<<definePINs[6], 1<<definePINs[7]};
	initLEDs(1+2+4,definePORTs,defineBITVALUEs);

	/* Insert system clock initialization code here (sysclk_init()). */
	volatile avr32_usart_t *usart = &AVR32_USART1;
	USART_init(usart);


	volatile char answer;
	while(1)
	{
		answer = USART_getChar();
		
		USART_putChar(answer);
	}
}

