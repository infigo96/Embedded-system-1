
#include "tasks.h"
__attribute__((__interrupt__)) static void readana(void)
{
	//Every time a Usart interupt for rxrdy is triggered this is run.
	xTaskResumeFromISR(pHandle);
	(&AVR32_USART1)->IMR.rxrdy;		//Reads the Interrupt Mask register to clear this interrupt.
	(&AVR32_USART1)->IDR.rxrdy = 1;

}


int main()
{
	//Set the main clock to 12MHz
	pm_switch_to_osc0(&AVR32_PM,FOSC0,OSC0_STARTUP); 
	//InitStuff
	initLED(); initBUTTON(); initUSART(); display_init();
	nQueue = 0;
	
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
	sizeQ = 8;
	Qhandle = xQueueCreate(sizeQ,1);
	xTaskCreate(Producer,"producer",configMINIMAL_STACK_SIZE,NULL,tskIDLE_PRIORITY + 1,&pHandle);
	//xTaskCreate(Consumer,"consumer",configMINIMAL_STACK_SIZE,NULL,tskIDLE_PRIORITY + 1,&cHandle);

	vTaskStartScheduler();
	
	return 0;
}