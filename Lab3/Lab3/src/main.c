# include "board.h"
# include "compiler.h"
#include "tc.h"
#include "avr32\uc3a0512.h"
#include "Stopwatch.h"

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


int main(void)
{
	// Initiate the global stopwatch time to 0.
	time = 0; 
	unsigned int channel = 0; //Compiler complains if 0 is written directly
	char timeString[3]; 

	// Initiate the LEDs
	initLED();


	volatile int i = 0;
	volatile int toggle = 0;
	volatile avr32_tc_t *tc = &AVR32_TC;
	// Initiates timer counter, sets CR compare interrupt 
	SW_init(tc);

	//Initiate USART for communication
	volatile avr32_usart_t *usart = &AVR32_USART1;
	USART_init(usart);

	// Reset function bound to button_0
	volatile avr32_gpio_port_t *a = &AVR32_GPIO.port[BUTTON0_PORT];
	volatile unsigned long btnstat;

	while(1)
	{
		btnstat = a->pvr & BUTTON0_PIN;
		if(btnstat == 0)
		{
			USART_reset();
		}

		char instruction = USART_getChar();

		if(instruction == 's')
		{
			toggle ^= 0x1 << 0;
			if(toggle == 1)
			{
				// Resets rise and start the counter. Look at Page 647 in data sheet.
				tc_start(tc,channel);
			}
			else
			{
				tc_stop(tc,channel);
			}
		}
		else if (instruction == 'r')
		{
			tc_stop(tc,channel);
			time = 0;
		}
		instruction = 0;

		Convert_Sec_To_String(timeString, time);
		//Debugging purposes
		i++;
	}
}