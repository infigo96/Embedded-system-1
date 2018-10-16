#ifndef LED_TASKS_H_
#define LED_TASKS_H_
#include "USART_driver.h"
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"


#include "tc.h"
#include <stdio.h>
#define NR_OF_TASKS 3
#define MARGIN 4
//The LED addresses
# define LED_PORT ( LED0_GPIO / 32 )
# define LED0_PIN 27
# define LED1_PIN 28
# define LED2_PIN 29

//The button addresses
#define BUTTON_PORT (GPIO_PUSH_BUTTON_0 >> 5)#define BUTTON0_PIN 1 << (GPIO_PUSH_BUTTON_0 & 0x1f)
#define BUTTON1_PIN 1 << (GPIO_PUSH_BUTTON_1 & 0x1f)
#define BUTTON2_PIN 1 << (GPIO_PUSH_BUTTON_2 & 0x1f)

#define sizeQ 16
#define nrTasks 4

//Struct used to check deadlines
typedef struct _task_struct
{
	xTaskHandle *pHandle;
	xTaskHandle *cHandle;
}task_struct;

typedef struct _task_info
{
	task_struct *Ts;
	int task_nr;
}Task_Info;


xSemaphoreHandle xSuspSemaphore;
xSemaphoreHandle GloReadSemaphore, GloTranSemaphore, GloQueueSemaphore;
xQueueHandle Qhandle;
int nQueue;
int nrProd,nrCons;

void initLED();
void initBUTTON(void);
void writeUSART_CRT(const char * message);
void Producer(void * pvParameters);
void Consumer(void * pvParameters);

#endif /* LED_TASKS_H_ */