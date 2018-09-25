#include "PWM_Led.h"
void delay(int ms)
{
	ms *=10;
	while(ms>0)
		ms--;
}
int main (void)
{
	initMAINCLK();
	initLED(GPIO_LED0_PIN);
	initLED(GPIO_LED1_PIN);
	initRTC();
	int hej;
	while(1){
		if (PWM(0.75))
			*(itp32(GPIO_LED_CONTROL + GPIO_OVR + CLR))|= (0x01 << GPIO_LED0_PIN); // Turn on LED
		else
			*(itp32(GPIO_LED_CONTROL + GPIO_OVR + SET))|= (0x01 << GPIO_LED0_PIN); // Turn off LED
		
		//delay(10);
	}
}
