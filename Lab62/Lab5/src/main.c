
#include "tasks.h"
__attribute__((__interrupt__)) static void readana(void)
{
	//Every time a Usart interupt for rxrdy is triggered this is run.
	xSemaphoreGiveFromISR( xSemaphore,&rHandle);
	//(&AVR32_USART1)->IMR.rxrdy;		//Reads the Interrupt Mask register to clear this interrupt.
	(&AVR32_USART1)->IDR.rxrdy = 1;

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
	
	vSemaphoreCreateBinary(xSemaphore);
	if( xSemaphore != NULL )
	{
		writeUSART("Semaphore created\r\n");
	}
	vSemaphoreCreateBinary(xSemaphore2);
	if( xSemaphore2 != NULL )
	{
		writeUSART("Semaphore2 created\r\n");
	}
	sizeQ = 8;
	charSum = 0;
	Qhandle = xQueueCreate(sizeQ,1);
	xTaskCreate(ReaderTask,"reader",configMINIMAL_STACK_SIZE,NULL,tskIDLE_PRIORITY + 1,&rHandle);
	xTaskCreate(StatusTask,"status",configMINIMAL_STACK_SIZE,NULL,tskIDLE_PRIORITY + 1,&sHandle);
	xTaskCreate(SwitchTask,"switch",configMINIMAL_STACK_SIZE,NULL,tskIDLE_PRIORITY + 2, NULL);
	
	vTaskSuspend(sHandle);
	vTaskStartScheduler();
	
	return 0;
}