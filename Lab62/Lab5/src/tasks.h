#ifndef LED_TASKS_H_
#define LED_TASKS_H_
#include "USART_driver.h"
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include "display_init.h"
#include "tc.h"
#include <string.h>
#include <stdio.h>


//The LED addresses
# define LED_PORT ( LED0_GPIO / 32 )
# define LED0_PIN 27
# define LED1_PIN 28
# define LED2_PIN 29

//The button addresses
#define BUTTON_PORT (GPIO_PUSH_BUTTON_0 >> 5)#define BUTTON0_PIN 1 << (GPIO_PUSH_BUTTON_0 & 0x1f)
#define BUTTON1_PIN 1 << (GPIO_PUSH_BUTTON_1 & 0x1f)
#define BUTTON2_PIN 1 << (GPIO_PUSH_BUTTON_2 & 0x1f)

//Struct used to check deadlines
typedef struct task_struct
{
	portTickType last_waketime;  //Stores return of vTaskDelayUntil()
	portTickType last_donetime;  //Stores the lastest time the task called vTaskDelayUntil() (task finnished execution)  
	portTickType task_period;    //The desired periodicity of the task. 
	portTickType next_period;    //
	portTickType start_delay;
}task_struct;

xSemaphoreHandle xSemaphoreReader,xSemaphoreStatus;
xQueueHandle Qhandle;
xTaskHandle rHandle,sHandle;
int nQueue;
int sizeQ;
char message[60];
unsigned int charSum;
void initLED();
void initBUTTON(void);
void writeUSART_CRT(const char * message);
void ReaderTask(void * pvParameters);
void StatusTask(void * pvParameters);
void SwitchTask(void * pvParameters);

#endif /* LED_TASKS_H_ */