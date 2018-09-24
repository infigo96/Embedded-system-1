#include "PWM_Led.h"

void initLED(void)
{
	
	*(itp32(GPIO_LED_CONTROL + GPIO_GPER ))|= (0x01 << GPIO_LED0_PIN);
	*(itp32(GPIO_LED_CONTROL + GPIO_ODER ))|= (0x01 << GPIO_LED0_PIN);
	*(itp32(GPIO_LED_CONTROL + GPIO_OVR ))|= ~(0x01 << GPIO_LED0_PIN);
	
}