
#include "PC_tasks.h"

int main()
{
	int i = 0;
	//Set the main clock to 12MHz
	pm_switch_to_osc0(&AVR32_PM,FOSC0,OSC0_STARTUP); 
	//InitStuff
	initLED(); initBUTTON(); initUSART(); 
	nQueue = 0;
	task_struct *TS = malloc(nrTasks*(sizeof(task_struct)));
	
	//Size of Queue buffer
	//Size of queue, size of a slot
	Qhandle = xQueueCreate(sizeQ,1);
	vSemaphoreCreateBinary(GloReadSemaphore);
	vSemaphoreCreateBinary(GloTranSemaphore);
	vSemaphoreCreateBinary(GloAllSemaphore);
	
	for(int i = 0; i < nrTasks; i++);
	{
		vSemaphoreCreateBinary((TS[i].xSemaphore));
		TS[i].PairNr = i;
		//Create tasks.
		xTaskCreate(Producer,"producer",configMINIMAL_STACK_SIZE,&(TS[i]),tskIDLE_PRIORITY + 1,&(TS[i].pHandle));
		xTaskCreate(Consumer,"consumer",configMINIMAL_STACK_SIZE,&(TS[i]),tskIDLE_PRIORITY + 1,&(TS[i].cHandle));
	}
	vTaskStartScheduler();
	
	return 0;
}