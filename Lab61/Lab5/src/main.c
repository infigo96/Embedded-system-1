
#include "PC_tasks.h"

int main()
{
	int i = 0;
	//Set the main clock to 12MHz
	pm_switch_to_osc0(&AVR32_PM,FOSC0,OSC0_STARTUP); 
	//InitStuff
	initLED(); initBUTTON(); initUSART(); 
	nQueue = 0;
	nrProd = 3;
	nrCons = 2;
	Task_Info *TI = malloc((nrProd+nrCons)*(sizeof(Task_Info)));
	task_struct *TS = malloc(sizeof(task_struct));
	TS->cHandle = malloc(nrCons*(sizeof(xTaskHandle)));
	TS->pHandle = malloc(nrProd*(sizeof(xTaskHandle)));
	
	//Size of Queue buffer
	//Size of queue, size of a slot
	Qhandle = xQueueCreate(sizeQ,1);
	vSemaphoreCreateBinary(GloReadSemaphore);
	vSemaphoreCreateBinary(GloTranSemaphore);
	vSemaphoreCreateBinary(GloQueueSemaphore);
	//vSemaphoreCreateBinary(xSuspSemaphore);
	xSuspSemaphore = xSemaphoreCreateMutex();
	
	for(int i = 0; i < nrProd; i++)
	{
		//Create tasks.
		TI[i].task_nr = i;
		TI[i].Ts = TS;
		xTaskCreate(Producer,"producer",configMINIMAL_STACK_SIZE,&(TI[i]),tskIDLE_PRIORITY + 1,&(TS->pHandle[i]));
	}
	for(int i = 0; i < nrCons; i++)
	{
		TI[i+nrProd].task_nr = i;
		TI[i+ nrProd].Ts = TS;
		xTaskCreate(Consumer,"consumer",configMINIMAL_STACK_SIZE,&(TI[i+nrProd]),tskIDLE_PRIORITY + 1,&(TS->cHandle[i]));
	}
	vTaskStartScheduler();
	
	return 0;
}