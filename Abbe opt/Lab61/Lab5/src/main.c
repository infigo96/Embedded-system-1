
#include "PC_tasks.h"


int main()
{
	//Set the main clock to 12MHz
	pm_switch_to_osc0(&AVR32_PM,FOSC0,OSC0_STARTUP); 
	//InitStuff
	initLED(); initBUTTON(); initUSART(); 
	nQueue = 0;
	
	vSemaphoreCreateBinary(xSemaphore);
	if( xSemaphore != NULL )
	{
		writeUSART("Semaphore created\r\n");
	}
	vSemaphoreCreateBinary(ySemaphore);
	if( ySemaphore != NULL )
	{
		writeUSART("Semaphore created\r\n");
	}
	//Size of Queue buffer
	sizeQ = 8;
	//Size of queue, size of a slot
	Qhandle = xQueueCreate(sizeQ,1);
	
	//Create tasks.
	xTaskCreate(Producer,"producer",configMINIMAL_STACK_SIZE,NULL,tskIDLE_PRIORITY + 1,&pHandle);
	xTaskCreate(Consumer,"consumer",configMINIMAL_STACK_SIZE,NULL,tskIDLE_PRIORITY + 1,&cHandle);

	vTaskStartScheduler();
	
	return 0;
}