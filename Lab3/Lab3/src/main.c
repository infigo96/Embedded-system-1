# include "board.h"
# include "compiler.h"
#include "..\tc\tc.h"
#include "avr32\uc3a0512.h"
/* Define the ports and pins to be used */
// The maximum number of pins in a port
# define GPIO_MAX_PIN_NUMBER 32

// This is the port which LED0_GPIO is located on
# define LED0_PORT ( LED0_GPIO / GPIO_MAX_PIN_NUMBER )
// This is the bit position of the GPIO pin
# define LED0_PIN ( LED0_GPIO & ( GPIO_MAX_PIN_NUMBER -1))
// This is a 1 - bit written to the bit position of the GPIO pin
# define LED0_BIT_VALUE (1 << LED0_PIN )

// This is the port which LED1_GPIO is located on
# define LED1_PORT ( LED1_GPIO / GPIO_MAX_PIN_NUMBER )
// This is the bit position of the GPIO pin
# define LED1_PIN ( LED1_GPIO & ( GPIO_MAX_PIN_NUMBER -1))
// This is a 1 - bit written to the bit position of the GPIO pin
# define LED1_BIT_VALUE (1 << LED1_PIN )
void initLED(void)
{
	//----------------------------------------LED0-----------------------------------------------------//
	//First make access to the GPIO port registers
	volatile avr32_gpio_port_t * led0_port;
	led0_port = & AVR32_GPIO.port[LED0_PORT];

	//Choose GPIO peripheral function. By writing a 1 to gpers , GPIO Enable Register Set
	led0_port->gpers = LED0_BIT_VALUE;

	//Set the output value register to 1 in order (turn the LED off)
	led0_port->ovrs = LED0_BIT_VALUE;

	//by writing a 1 to oders , Output Drive Enable
	led0_port->oders = LED0_BIT_VALUE;
	
	
	//----------------------------------------LED1-----------------------------------------------------//
	//First make access to the GPIO port registers
	volatile avr32_gpio_port_t * led1_port;
	led1_port = & AVR32_GPIO.port[LED1_PORT];

	//Choose GPIO peripheral function. By writing a 1 to gpers , GPIO Enable Register Set
	led1_port->gpers = LED1_BIT_VALUE;

	//Set the output value register to 1 in order (turn the LED off)
	led1_port->ovrs = LED1_BIT_VALUE;

	//by writing a 1 to oders , Output Drive Enable
	led1_port->oders = LED1_BIT_VALUE;
}

__attribute__((__interrupt__)) static void tc_irq_handler(void)
{
	AVR32_GPIO.port[LED0_PORT].ovrt = LED0_BIT_VALUE;
	tc_read_sr(&AVR32_TC, 0);
}
int main(void)
{
	initLED();
	/* Main loop that will toggle a single bit on the GPIO port
	*/
	volatile int i = 0;
	volatile int toggle = 0;
	volatile  tc = &AVR32_TC;
	volatile tc_waveform_opt_t *opt;
	opt->channel = 0;
	opt->wavsel = TC_WAVEFORM_SEL_UP_MODE_RC_TRIGGER;
	opt->tcclks = TC_CLOCK_SOURCE_XC0;
	int tc_init_waveform(tc, opt);
	unsigned int hejsan = 0;
	int tc_start(tc, hejsan);
	INTC_register_interrupt(&tc_irq_handler,0,AVR32_INTC_INT0);
	
	while(1) //both part 1 and 2s function condition is checked. 
	{
		
		
		//Debugging purposes
		i++;
	}
}