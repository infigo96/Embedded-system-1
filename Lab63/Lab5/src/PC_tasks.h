#ifndef LED_TASKS_H_
#define LED_TASKS_H_
#include "USART_driver.h"
#include "adc.h"
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include <stdio.h>
#include "tc.h"

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

typedef struct prod_handle_struct
{
	xTaskHandle tHandle,lHandle,pHandle;
}prod_handle_struct;

//Struct used to check deadlines
typedef struct task_struct
{
	xSemaphoreHandle consumerSuspendSemaphore, writeQueueSemaphore;
	xTaskHandle cHandle;
	prod_handle_struct producerHandles;
}task_struct;

typedef struct value_type_struct
{
	int taskValue;
	char taskType;
}value_type_struct;

xQueueHandle Qhandle;
int nQueue;
int sizeQ;

void initLED();
void initBUTTON(void);
void writeUSART_CRT(const char * message);
void potentSensorTask(void * pvParameters);
void lightSensorTask(void * pvParameters);
void tempSensorTask(void * pvParameters);
void sensorReaderTask(void * pvParameters);

#endif /* LED_TASKS_H_ */