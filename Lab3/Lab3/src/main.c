# include "board.h"
# include "compiler.h"
#include "tc.h"
#include "avr32\uc3a0512.h"
#include "Stopwatch.h"
#include "USART_driver.h"
#include "Utilities.h"

//Initializes the LED
void initLED(void);
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
	char instruction = 0;
	int localTime = 0; //Used to detect if an interrupt has occurred
	unsigned int channel = 0; //Compiler complains if 0 is written directly
	char timeString[25]; //Stores the formated time

	// Initiate the LEDs
	initLED();


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


		instruction = USART_getChar(); //Read instruction from user (if any exists)

		//if the instruction is a Start/Stop command
		if(instruction == 's')
		{
			toggle ^= 0x1 << 0; //Changes the toggle variable between 1 and 0
			if(toggle == 1)
			{
				// Resets rise and start the counter. Look at Page 647 in data sheet.
				SW_start(tc,channel);
				instruction = 0; //Reset the instruction to avoid re-enter the if case

			}
			else
			{
				instruction = 0; //Reset the instruction to avoid re-enter the if case
				SW_stop(tc,channel);
			}
		}
		//if the instruction is a Reset command
		else if (instruction == 'r')
		{
			tc_stop(tc,channel);
			time = 0;
			toggle=0;
			instruction = 0; //Reset the instruction to avoid re-enter the if case

		}
		
		//If one whole second has passed
		if ((int)(time/10) != localTime)
		{
			AVR32_GPIO.port[LED0_PORT].ovrt = LED0_BIT_VALUE; //Toggle the LED
			localTime = (int)(time/10); //Convert the clock time into seconds
			Convert_Sec_To_String(timeString, localTime); //Format the time into a string ("00h 00m 00s") with ANSI escape sequences
			USART_putString(timeString); //Send the string through USART
		}
	}
}