
#include "PC_tasks.h"


int main()
{
	//Set the main clock to 12MHz
	pm_switch_to_osc0(&AVR32_PM,FOSC0,OSC0_STARTUP); 
	//InitStuff
	initLED(); initBUTTON(); initUSART(); 
	nQueue = 0;
	task_struct *TS;
	char MS1[] = "Philosohper x take left fork\n";
	char MS2[] = "Philosohper x take right fork and start eating\n";
	char MS3[] = "Philosohper x stop eating and give right fork\n";
	char MS4[] = "Philosohper x give left fork and starts thinking\n";


	//Size of Queue buffer
	sizeQ = 8;
	//Size of queue, size of a slot
	Qhandle = xQueueCreate(sizeQ,1);
	Phil_Struct *Structs = (Phil_Struct*)malloc(sizeof(Phil_Struct)*5);
	xSemaphoreHandle Semaphores[5];
	vSemaphoreCreateBinary(Semaphores[0]);	
	vSemaphoreCreateBinary(Semaphores[1]);
	vSemaphoreCreateBinary(Semaphores[2]);
	vSemaphoreCreateBinary(Semaphores[3]);
	vSemaphoreCreateBinary(Semaphores[4]);
	vSemaphoreCreateBinary(UsartSem);
	
	vSemaphoreCreateBinary((TS->xSemaphore));
	
	//Create tasks.
	Structs[0].lFork = Semaphores[4];
	Structs[0].rFork = Semaphores[0];
	Structs[0].nrPh = 0;
	Structs[0].MTLF = MS1;
	Structs[0].MTRF = MS2;
	Structs[0].MGRF = MS3;
	Structs[0].MGLF = MS4;
	xTaskCreate(Philosopher,"Philosopher",configMINIMAL_STACK_SIZE,&(Structs[0]),tskIDLE_PRIORITY + 1,NULL);
	
	for(int i=0; i<4; i++)
	{
		Structs[i+1].lFork = Semaphores[i];
		Structs[i+1].rFork = Semaphores[i+1];
		Structs[i+1].nrPh = i+1;
		Structs[i+1].MTLF = MS1;
		Structs[i+1].MTRF = MS2;
		Structs[i+1].MGRF = MS3;
		Structs[i+1].MGLF = MS4;
		xTaskCreate(Philosopher,"Philosopher",configMINIMAL_STACK_SIZE,&(Structs[i+1]),tskIDLE_PRIORITY + 1,NULL);
	}
	
	//xTaskCreate(Producer,"producer",configMINIMAL_STACK_SIZE,TS,tskIDLE_PRIORITY + 1,&(TS->pHandle));
	//xTaskCreate(Consumer,"consumer",configMINIMAL_STACK_SIZE,TS,tskIDLE_PRIORITY + 1,&(TS->cHandle));

	vTaskStartScheduler();
	
	return 0;
}