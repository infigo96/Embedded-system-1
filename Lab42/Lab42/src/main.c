#include "PWM_Led.h"
int main (void)
{
	initMAINCLK(); // Sets the main clock to 12MHz to improve stability of the generated PWM
	//Initialize the LEDs
	initLED(GPIO_LED0_PIN); //Used for showing the PWM
	initLED(GPIO_LED1_PIN); //Reference light
	initRTC(); // Setup the real time clock to 32KHz (generates the PWM)
	float duty_cycle = 0.2;
	while(1){
		*(itp32(GPIO_LED_CONTROL + GPIO_OVR)) ^= ((-PWM(duty_cycle) ^ *(itp32(GPIO_LED_CONTROL + GPIO_OVR))) & (0x1 << GPIO_LED0_PIN));
		
		//Old code that does the same as the above ^
		/*
		if (PWM(0.75))
			*(itp32(GPIO_LED_CONTROL + GPIO_OVR + CLR))|= (0x01 << GPIO_LED0_PIN); // Turn on LED
		else
			*(itp32(GPIO_LED_CONTROL + GPIO_OVR + SET))|= (0x01 << GPIO_LED0_PIN); // Turn off LED
		*/

	}
}
