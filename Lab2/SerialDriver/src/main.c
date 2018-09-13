# include "board.h"
# include "compiler.h"
# include "driver.h"
# include "LED.h"
void mdelay(int ms);
void mdelay(int ms)
{
	long volatile cycles = 10*ms;
	while (cycles != 0)
	{
		cycles--;
	}
}

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
	if(usart->CSR.txrdy==1) //Is transmitter ready?
	{
		lightLED(1,definePORTs,defineBITVALUEs);
	}
	volatile char answer = 'r';
	while(1)
	{
		mdelay(500);
		USART_putChar(answer);
		//if(usart->CSR.txrdy==1)
		//{
			//lightLED(2,definePORTs,defineBITVALUEs);
		//}
		
		answer = USART_getChar();
		if(answer != 'b' && answer !=  0)
		{
			lightLED(4,definePORTs,defineBITVALUEs);
			closeLED(2,definePORTs,defineBITVALUEs);
		}
		else
		{
			lightLED(2,definePORTs,defineBITVALUEs);
		}

	}
	/* Insert application code here, after the board has been initialized. */
}
