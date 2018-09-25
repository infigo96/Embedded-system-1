# include "board.h"
# include "compiler.h"
#include "tc.h"
#include "avr32\uc3a0512.h"
#include "Stopwatch.h"
#include "USART_driver.h"
#include "Utilities.h"
#include "LED.h"

//Initializes the LED
int binbun(void)
{
	int RetValue = 0;
	int number_table[7] = {128,64,32,16,8,4,2};
	for(int i = 0; i < 7; i++)
	{
		if(buttin[i] == 0)
		{
			RetValue += number_table[i];
		}
	}
	return RetValue;
}

int main(void)
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
	initLEDs(255,definePORTs,defineBITVALUEs);
	
	
	
	// Initiate the global stopwatch time to 0.
	time = 0; 
	char instruction = 0;

	int localTime = 0; //Used to detect if an interrupt has occurred
	unsigned int channel = 0; //Compiler complains if 0 is written directly
	char timeString[25]; //Stores the formated time

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
	int ledNumbers; 
	unsigned int endPress = 0;
	int start = 0;
	while(1)
	{
		
		if(lockdown == 1)
		{
			AVR32_GPIO.port[LED0_PORT].ovrt = LED0_BIT_VALUE;
			ledNumbers = binbun();
			lightLED(ledNumbers,definePORTs,defineBITVALUEs);
			closeLED(254-ledNumbers,definePORTs,defineBITVALUEs);
			start = 0; 
			for(int i = 0; i<7; i++)
			{
				if(buttin[i] == 0 && start == 0)
				{
					start = 1;
				}
				else if (buttin[i] != 0 && start == 1)
				{
					lockdown = 2;
				}
			}
			do
			{
				endPress= hej->pvr & BUTTON0_PIN;
			}
			while(endPress == 0 || lockdown == 2);
			
			lockdown = 0;
			tc_start(tc,0);
		}
		i++;
	}
}