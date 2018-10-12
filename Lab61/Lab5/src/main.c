
#include "PC_tasks.h"


int main()
{
	//Set the main clock to 12MHz
	pm_switch_to_osc0(&AVR32_PM,FOSC0,OSC0_STARTUP); 
	//InitStuff
	initLED(); initBUTTON(); initUSART(); 
	
	
	/*vSemaphoreCreateBinary(xSemaphore);
	if( xSemaphore != NULL )
	{
		writeUSART("Semaphore created\r\n");
	}*/

	Qhandle = xQueueCreate(4,1);
	xTaskCreate(Producer,"producer",configMINIMAL_STACK_SIZE,NULL,tskIDLE_PRIORITY + 1,NULL);
	xTaskCreate(Consumer,"consumer",configMINIMAL_STACK_SIZE,NULL,tskIDLE_PRIORITY + 1,NULL);

	vTaskStartScheduler();
	
	return 0;
}