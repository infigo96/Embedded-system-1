# include "board.h"
# include "compiler.h"
#include "tc.h"
#include "avr32\uc3a0512.h"
#include "Stopwatch.h"
#include "USART_driver.h"
#include "Utilities.h"
#include "GPIO_INTERFACE.h"
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

	// Initiate the LEDs
	initLED();
	GPIO_init();
	
	volatile avr32_tc_t *tc = &AVR32_TC;
	// Initiates timer counter, sets CR compare interrupt 
	SW_init(tc);

	////Initiate USART for communication
	//volatile avr32_usart_t *usart = &AVR32_USART1;
	//USART_init(usart);
	volatile unsigned long joystick_state;
	volatile unsigned long button_state;
	int previus_state = 0;
	currentTime=0;
	unsigned long lastTime=0;
	const int delay = 1000;
	tc_start(tc,0); //Start the timer
	volatile avr32_gpio_port_t *joystick_port = &AVR32_GPIO.port[JOYSTICK_PORT];
	volatile avr32_gpio_port_t *button_port = &AVR32_GPIO.port[BUTTON_PORT];
	volatile int pushes = 0;
	while(1)
	{
		//button_state = stickport->pvr & GPIO_JOYSTICK_DOWN_PIN;
		joystick_state = joystick_port->pvr & JOYSTICK_PIN;
		button_state = button_port->pvr & BUTTON_PIN;
		//Check if an signal is read from the button
		if(joystick_state==0 && previus_state == 0)
		{
			if((currentTime - delay) < lastTime){
				AVR32_GPIO.port[LED0_PORT].ovrc = LED0_BIT_VALUE;
			}
			lastTime = currentTime;
			
		}
		if(button_state==0){
			AVR32_GPIO.port[LED0_PORT].ovrs = LED0_BIT_VALUE;
		}
		previus_state = joystick_state;
	}
}