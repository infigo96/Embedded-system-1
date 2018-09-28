
#include "led_tasks.h"
#include "tc.h"
#include "pm.h"
#include "power_clocks_lib.h"

int main()
{

	pcl_switch_to_osc(PCL_OSC0,FOSC0,OSC0_STARTUP); //Set the main clock to 12MHz
	//Init the GPIO units
	initLED(); initBUTTON();
	
	//The tasks for handling blinking of the three LEDs
	xTaskCreate(vBlinkLED1,"Blink1",configMINIMAL_STACK_SIZE,NULL,tskIDLE_PRIORITY + 2,&xTaskHandle_blink1);
	xTaskCreate(vBlinkLED2,"Blink2",configMINIMAL_STACK_SIZE,NULL,tskIDLE_PRIORITY + 2,&xTaskHandle_blink2);
	xTaskCreate(vBlinkLED3,"Blink3",configMINIMAL_STACK_SIZE,NULL,tskIDLE_PRIORITY + 2,&xTaskHandle_blink3);
	
	//The tasks that sets the three LEDs
	xTaskCreate(vLightLED1,"Light1",configMINIMAL_STACK_SIZE,NULL,tskIDLE_PRIORITY + 2,&xTaskHandle_light1);
	xTaskCreate(vLightLED2,"Light2",configMINIMAL_STACK_SIZE,NULL,tskIDLE_PRIORITY + 2,&xTaskHandle_light2);
	xTaskCreate(vLightLED3,"Light3",configMINIMAL_STACK_SIZE,NULL,tskIDLE_PRIORITY + 2,&xTaskHandle_light3);
	
	xTaskCreate(vReadButtons,"Buttons",configMINIMAL_STACK_SIZE,NULL,tskIDLE_PRIORITY + 2,NULL);
	//Suspend the task so the lights starts blinking
	vTaskSuspend(xTaskHandle_light1); 
	vTaskSuspend(xTaskHandle_light2);
	vTaskSuspend(xTaskHandle_light3); 
	vTaskStartScheduler();
	
	return 0;
}