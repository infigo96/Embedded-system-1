#include "PWM_Led.h"
int main (void)
{
	initMAINCLK();
	initLED(GPIO_LED0_PIN);
	initLED(GPIO_LED1_PIN);
	initRTC();
	volatile int hej;
	while(1){
		*(itp32(GPIO_LED_CONTROL + GPIO_OVR)) ^= ((-PWM(0.75) ^ *(itp32(GPIO_LED_CONTROL + GPIO_OVR))) & (0x1 << GPIO_LED0_PIN));
		
		//if (PWM(0.75))
			//*(itp32(GPIO_LED_CONTROL + GPIO_OVR + CLR))|= (0x01 << GPIO_LED0_PIN); // Turn on LED
		//else
			//*(itp32(GPIO_LED_CONTROL + GPIO_OVR + SET))|= (0x01 << GPIO_LED0_PIN); // Turn off LED

	}
}
