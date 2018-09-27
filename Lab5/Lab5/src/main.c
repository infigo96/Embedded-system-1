
#include "led_tasks.h"
#include "tc.h"
#include "pm.h"
#include "power_clocks_lib.h"

int main()
{
	pcl_switch_to_osc(PCL_OSC0,FOSC0,OSC0_STARTUP);
	initLED();
	AVR32_GPIO.port[LED_PORT].ovrt = (1 << LED0_PIN);

	xTaskCreate(vBlinkLED1,"Blink1",configMINIMAL_STACK_SIZE,NULL,tskIDLE_PRIORITY + 1,NULL);

	AVR32_GPIO.port[LED_PORT].ovrt = (1 << LED0_PIN);
	
	vTaskStartScheduler();
	
	return 0;
}