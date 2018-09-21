# include "board.h"
# include "compiler.h"
#include "tc.h"
#include "avr32\uc3a0512.h"
#include "Stopwatch.h"
#include "USART_driver.h"
#include "Utilities.h"

//Initializes the LED
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
	presses[0] = 0;
	presses[1] = 0;
	unsigned int buttonval[6];
	firstPress = 0;
	int localTime = 0; //Used to detect if an interrupt has occurred
	unsigned int channel = 0; //Compiler complains if 0 is written directly
	char timeString[25]; //Stores the formated time

	// Initiate the LEDs
	initLED();
	// Initiate reset button for USART.
	volatile avr32_gpio_port_t * button0_port;
	button0_port = &AVR32_GPIO.port[BUTTON0_PORT];
	button0_port->gpers = BUTTON0_PIN;
	button0_port->oderc = BUTTON0_PIN;
	
	volatile int i = 0;
	volatile int toggle = 0;
	lockdown = 0;
	volatile avr32_tc_t *tc = &AVR32_TC;
	// Initiates timer counter, sets CR compare interrupt 
	SW_init(tc);
	hej = &(AVR32_GPIO.port[BUTTON0_PORT]);
	//Initiate USART for communication
	volatile avr32_usart_t *usart = &AVR32_USART1;
	USART_init(usart);
	tc_start(tc,0);
	// Reset function bound to button_0
	volatile avr32_gpio_port_t *a = &AVR32_GPIO.port[BUTTON0_PORT];
	volatile unsigned long btnstat;
	AVR32_GPIO.port[LED1_PORT].ovrt = LED1_BIT_VALUE;
	while(1)
	{
		/*if(((int)time/10) != localTime) //Timer for delay, run tc5 and RC on 56250 for one sec trig
		{
			AVR32_GPIO.port[LED0_PORT].ovrt = LED0_BIT_VALUE;
			localTime = (int)time/10;

		}*/
		if(lockdown == 1)
		{
			AVR32_GPIO.port[LED0_PORT].ovrt = LED0_BIT_VALUE;
			//tc_stop(tc,0);
			for(int i; i < 6; i++)
			{
				buttonval[i] = buttin[i] & BUTTON0_PIN;
			}
			lockdown == 0;
		}
		i++;
	}
}