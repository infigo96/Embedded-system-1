
#include "led_tasks.h"
#include "tc.h"
#include "pm.h"
#include "power_clocks_lib.h"

int main()
{

	pcl_switch_to_osc(PCL_OSC0,FOSC0,OSC0_STARTUP); //Set the main clock to 12MHz
	//Init the GPIO units
	initLED(); initBUTTON();
	xTaskHandle *xTaskHandle_blink = malloc(sizeof(xTaskHandle)*3);
	xTaskHandle *xTaskHandle_light = malloc(sizeof(xTaskHandle)*3);

	//The tasks for handling blinking of the three LEDs
	xTaskCreate(vBlinkLED1,"Blink1",configMINIMAL_STACK_SIZE,NULL,tskIDLE_PRIORITY + 2,&(xTaskHandle_blink[0]));
	xTaskCreate(vBlinkLED2,"Blink2",configMINIMAL_STACK_SIZE,NULL,tskIDLE_PRIORITY + 2,&(xTaskHandle_blink[1]));
	xTaskCreate(vBlinkLED3,"Blink3",configMINIMAL_STACK_SIZE,NULL,tskIDLE_PRIORITY + 2,&(xTaskHandle_blink[2]));
	
	//The tasks that sets the three LEDs
	xTaskCreate(vLightLED1,"Light1",configMINIMAL_STACK_SIZE,&(xTaskHandle_blink[0]),tskIDLE_PRIORITY + 2,&(xTaskHandle_light[0]));
	xTaskCreate(vLightLED2,"Light2",configMINIMAL_STACK_SIZE,&(xTaskHandle_blink[1]),tskIDLE_PRIORITY + 2,&(xTaskHandle_light[1]));
	xTaskCreate(vLightLED3,"Light3",configMINIMAL_STACK_SIZE,&(xTaskHandle_blink[2]),tskIDLE_PRIORITY + 2,&(xTaskHandle_light[2]));
	
	xTaskCreate(vReadButtons,"Buttons",configMINIMAL_STACK_SIZE,xTaskHandle_light,tskIDLE_PRIORITY + 2,NULL);
	//Suspend the task so the lights starts blinking
	vTaskSuspend(xTaskHandle_light[0]); 
	vTaskSuspend(xTaskHandle_light[1]);
	vTaskSuspend(xTaskHandle_light[2]); 
	vTaskStartScheduler();
	
	return 0;
}