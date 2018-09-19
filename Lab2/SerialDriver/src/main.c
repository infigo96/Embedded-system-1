# include "board.h"
# include "compiler.h"
# include "driver.h"
# include "LED.h"


int main (void)
{

	// Some setup for using initLED(), lightLED() and closeLED().
	// First is the port numbers
	int definePORTs[8] = {	LED0_GPIO / GPIO_MAX_PIN_NUMBER, LED1_GPIO / GPIO_MAX_PIN_NUMBER,
							LED2_GPIO / GPIO_MAX_PIN_NUMBER, LED3_GPIO / GPIO_MAX_PIN_NUMBER,
							LED4_GPIO / GPIO_MAX_PIN_NUMBER, LED5_GPIO / GPIO_MAX_PIN_NUMBER,
							LED6_GPIO / GPIO_MAX_PIN_NUMBER, LED7_GPIO / GPIO_MAX_PIN_NUMBER};
	// Then given port, calc the pin for that port.
	int definePINs[8] = {	LED0_GPIO & ( GPIO_MAX_PIN_NUMBER -1), LED1_GPIO & ( GPIO_MAX_PIN_NUMBER -1),
							LED2_GPIO & ( GPIO_MAX_PIN_NUMBER -1), LED3_GPIO & ( GPIO_MAX_PIN_NUMBER -1),
							LED4_GPIO & ( GPIO_MAX_PIN_NUMBER -1), LED5_GPIO & ( GPIO_MAX_PIN_NUMBER -1),
							LED6_GPIO & ( GPIO_MAX_PIN_NUMBER -1), LED7_GPIO & ( GPIO_MAX_PIN_NUMBER -1)};
	// Finally create a bit mask with a 1 shifted to desired pin.
	int defineBITVALUEs[8] = {	1<<definePINs[0], 1<<definePINs[1], 1<<definePINs[2], 1<<definePINs[3],
								1<<definePINs[4], 1<<definePINs[5], 1<<definePINs[6], 1<<definePINs[7]};
	
	// Initiate the LEDs we require
	initLEDs(1+2+4,definePORTs,defineBITVALUEs);
	
	// Initiate reset button.
	volatile avr32_gpio_port_t * button0_port;
	button0_port = &AVR32_GPIO.port[BUTTON0_PORT];
	button0_port->gpers = BUTTON0_PIN;
	button0_port->oderc = BUTTON0_PIN;

	// Initiate Usart.
	volatile avr32_usart_t *usart = &AVR32_USART1;
	USART_init(usart);

	// Message buffer handled by the operating system
	char Message[BUFFERSIZE];
	while(1)
	{
		// Gets string and returns it
		USART_getString(Message, BUFFERSIZE);
		USART_putString(Message, BUFFERSIZE);
	}
}

