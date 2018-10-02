
#include "led_tasks.h"
int main()
{

	pm_switch_to_osc0(&AVR32_PM,FOSC0,OSC0_STARTUP); //Set the main clock to 12MHz
	//Init the GPIO units
	initLED(); initBUTTON();initUSART();

	//Allocate the space for 
	task_struct * t_s = (task_struct*)calloc(NR_OF_TASKS,sizeof(task_struct));
	//The tasks for handling blinking of the three LEDs
	xTaskCreate(vBlinkLED1,"Blink1",configMINIMAL_STACK_SIZE,NULL,tskIDLE_PRIORITY + 2,NULL);
	xTaskCreate(vBlinkLED2,"Blink2",configMINIMAL_STACK_SIZE,NULL,tskIDLE_PRIORITY + 2,NULL);
	xTaskCreate(vBlinkLED3,"Blink3",configMINIMAL_STACK_SIZE,NULL,tskIDLE_PRIORITY + 2,NULL);
	

	
	xTaskCreate(vReadButtons,"Buttons",configMINIMAL_STACK_SIZE,NULL,tskIDLE_PRIORITY + 2,NULL);

	vTaskStartScheduler();
	
	return 0;
}