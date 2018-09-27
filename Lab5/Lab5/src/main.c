
#include "led_tasks.h"
#include "tc.h"
#include "pm.h"
#include "power_clocks_lib.h"

int main()
{
	xTaskHandle xHandle1;
	xTaskHandle xHandle2;
	pcl_switch_to_osc(PCL_OSC0,FOSC0,OSC0_STARTUP);
	initLED();
	
	xTaskCreate(vBlinkLED1,"Blink1",configMINIMAL_STACK_SIZE,NULL,tskIDLE_PRIORITY + 2,NULL);
	xTaskCreate(vBlinkLED2,"Blink2",configMINIMAL_STACK_SIZE,NULL,tskIDLE_PRIORITY + 2,NULL);
	xTaskCreate(vBlinkLED3,"Blink3",configMINIMAL_STACK_SIZE,NULL,tskIDLE_PRIORITY + 2,NULL);
	
	vTaskStartScheduler();
	
	return 0;
}