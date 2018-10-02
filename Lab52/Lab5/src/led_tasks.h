#ifndef LED_TASKS_H_
#define LED_TASKS_H_
#include "USART_driver.h"
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"

#include "tc.h"
#include <stdio.h>
#define NR_OF_TASKS 3

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
	portTickType last_waketime;
	portTickType task_period;
}task_struct;

void initLED();
void initBUTTON();
void vReadButtons(void * pvParameters);
void vBlinkLED1( void * pvParameters );
void vBlinkLED2( void * pvParameters );
void vBlinkLED3( void * pvParameters );
void vLightLED1( void * pvParameters );
void vLightLED2( void * pvParameters );
void vLightLED3( void * pvParameters );
void writeUSART_CRT(const char * message);
#endif /* LED_TASKS_H_ */