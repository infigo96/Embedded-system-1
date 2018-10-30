
#include "PC_tasks.h"


int main()
{
	//Set the main clock to 12MHz
	pm_switch_to_osc0(&AVR32_PM,FOSC0,OSC0_STARTUP); 
	//InitStuff
	initLED(); initBUTTON(); initUSART();
	int * ids = (int*)calloc(NR_OF_PHIL,sizeof(int));
	vSemaphoreCreateBinary(writeSemaphore);
	if(writeSemaphore != NULL){
		writeUSART("Semaphore created\r\n");
	}
	for (int i = 0; i < NR_OF_PHIL; i++)
	{
		ids[i] = i;
		vSemaphoreCreateBinary(xSemaphore[i]);
		if( xSemaphore[i] != NULL )
		{
			writeUSART("Semaphore created\r\n");
		}
		else
		{
			break;
		}
		xTaskCreate(Philosopher,"phil",configMINIMAL_STACK_SIZE,&(ids[i]),tskIDLE_PRIORITY + 2,NULL);	
	}
	

	vTaskStartScheduler();
	
	return 0;
}