#include "PWM_Led.h"
int main (void)
{
	initMAINCLK(); // Sets the main clock to 12MHz to improve stability of the generated PWM
	//Initialize the LEDs
	initLED(GPIO_LED0_PIN); //Used for showing the PWM
	initLED(GPIO_LED1_PIN); //Reference light
	initRTC(); // Setup the real time clock to 32KHz (generates the PWM)
	float duty_cycle = 0.2;
	int i = 0; 
	int lastPWM = 0; 
	int rPWM;
	int lookup_table[] = {12,12,13,13,14,14,15,15,16,16,16,17,17,18,18,18,19,19,19,20,20,20,21,21,21,21,22,22,22,22,22,
		23,23,23,23,23,23,23,23,23,23,23,23,23,23,23,22,22,22,22,22,21,21,21,21,20,20,20,19,19,19,18,18,
		18,17,17,16,16,16,15,15,14,14,13,13,12,12,12,11,11,10,10,9,9,8,8,7,7,7,6,6,5,5,5,4,
		4,4,3,3,3,2,2,2,2,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,
		1,1,2,2,2,2,3,3,3,4,4,4,5,5,5,6,6,7,7,7,8,8,9,9,10,10,11,11,12,12};	//156 datapoints of 0-23
	
	
	while(1)
	{
		
		rPWM = PWM(lookup_table[i]); //read if led should be on (0) or off (1)
		if(rPWM < lastPWM)		//If we last have wrapped down to on state again 
		{
			i++;
			if(i == 156)		//Back to 0 if we reach wrap value. 
			{
				i = 0;
			}
		}
		lastPWM = rPWM;
		*(itp32(GPIO_LED_CONTROL + GPIO_OVR)) ^= (((-rPWM) ^ *(itp32(GPIO_LED_CONTROL + GPIO_OVR))) & (0x1 << GPIO_LED0_PIN));
		
	}
}
