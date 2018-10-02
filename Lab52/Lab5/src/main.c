
#include "led_tasks.h"
int main()
{

	pm_switch_to_osc0(&AVR32_PM,FOSC0,OSC0_STARTUP); //Set the main clock to 12MHz
	initLED(); initBUTTON();initUSART(); //InitStuff



	//Allocate the space for the task_struct array which contains time information about the tasks
	task_struct * t_s = (task_struct*)calloc(NR_OF_TASKS,sizeof(task_struct));
	vSemaphoreCreateBinary(xSemaphore);
	if( xSemaphore != NULL )
	{
		writeUSART("Semaphore created\r\n");
	}
	//Set th periods for the tasks
	configTaskTime(6000,0, &(t_s[0]));
	configTaskTime(4000,700, &(t_s[1]));
	configTaskTime(1000,500, &(t_s[2]));
	
	//The tasks for handling blinking of the three LEDs
	xTaskCreate(vBlinkLED1,"Blink1",configMINIMAL_STACK_SIZE,&(t_s[0]),tskIDLE_PRIORITY + 1,NULL);
	xTaskCreate(vBlinkLED2,"Blink2",configMINIMAL_STACK_SIZE,&(t_s[1]),tskIDLE_PRIORITY + 2,NULL);
	xTaskCreate(vBlinkLED3,"Blink3",configMINIMAL_STACK_SIZE,&(t_s[2]),tskIDLE_PRIORITY + 3,NULL);
	//xTaskCreate(vReadButtons,"Buttons",configMINIMAL_STACK_SIZE,NULL,tskIDLE_PRIORITY + 4,NULL);
//
	xTaskCreate(vOverseer,"Overseer",configMINIMAL_STACK_SIZE,t_s,tskIDLE_PRIORITY + 5,NULL);

	vTaskStartScheduler();
	
	return 0;
}