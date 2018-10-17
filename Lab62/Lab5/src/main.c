
#include "tasks.h"
__attribute__((__interrupt__)) static void readana(void)
{
	//Every time a Usart interupt for rxrdy is triggered this is run.
	xSemaphoreGiveFromISR( xSemaphoreReader,&rHandle);		//This releases the reader to read the message.
	(&AVR32_USART1)->IMR.rxrdy;		//Reads the Interrupt Mask register to clear this interrupt.
	(&AVR32_USART1)->IDR.rxrdy = 1;	//Disables this the interrupt itself the reader can read the message without this bothering. 

}


int main()
{
	//Set the main clock to 12MHz
	pm_switch_to_osc0(&AVR32_PM,FOSC0,OSC0_STARTUP); 
	//InitStuff
	initLED(); initBUTTON(); initUSART(); display_init();
	nQueue = 0;
	charSum = 0;
	
	Disable_global_interrupt();
	INTC_init_interrupts();
	INTC_register_interrupt(&readana, AVR32_USART1_IRQ, AVR32_INTC_INT0);
	(&AVR32_USART1)->IER.rxrdy = 1;
	
	Enable_global_interrupt();
	
	vSemaphoreCreateBinary(xSemaphoreReader);	//Semaphore for the reader task
	if( xSemaphoreReader != NULL )
	{
		writeUSART("Semaphore created\r\n");
	}
	vSemaphoreCreateBinary(xSemaphoreStatus);	//Semaphore for the status task
	if( xSemaphoreStatus != NULL )
	{
		writeUSART("Semaphore created\r\n");
	}
	sizeQ = 8;	//Size of the Queue
	charSum = 0;
	Qhandle = xQueueCreate(sizeQ,1);
	xTaskCreate(ReaderTask,"reader",configMINIMAL_STACK_SIZE,NULL,tskIDLE_PRIORITY + 1,&rHandle);
	xTaskCreate(StatusTask,"status",configMINIMAL_STACK_SIZE,NULL,tskIDLE_PRIORITY + 1,&sHandle);
	xTaskCreate(SwitchTask,"switch",configMINIMAL_STACK_SIZE,NULL,tskIDLE_PRIORITY + 2, NULL);	//Button reader that switch to activate the status have higher priority
	
	vTaskSuspend(sHandle);	//The status task will run when it is asked to run by the switch task. 
	vTaskStartScheduler();
	
	return 0;
}