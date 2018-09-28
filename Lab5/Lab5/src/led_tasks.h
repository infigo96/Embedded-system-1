/*
 * led_tasks.h
 *
 * Created: 2018-09-27 11:19:59
 *  Author: Albert
 */ 


#ifndef LED_TASKS_H_
#define LED_TASKS_H_
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include "gpio.h"
#include "pm.h"
#include "usart.h"

//The LED addresses
# define LED_PORT ( LED0_GPIO / 32 )
# define LED0_PIN 27
# define LED1_PIN 28
# define LED2_PIN 29

//The button addresses
#define BUTTON_PORT (GPIO_PUSH_BUTTON_0 >> 5)#define BUTTON0_PIN (GPIO_PUSH_BUTTON_0 & 0x1f)
#define BUTTON1_PIN (GPIO_PUSH_BUTTON_1 & 0x1f)
#define BUTTON2_PIN (GPIO_PUSH_BUTTON_2 & 0x1f)

//Used to ref back to the tasks

int status[3]; //Saves the current state of the three LEDs

void initLED();
void initBUTTON();
void vReadButtons(void * pvParameters);
void vBlinkLED1( void * pvParameters );
void vBlinkLED2( void * pvParameters );
void vBlinkLED3( void * pvParameters );
void vLightLED1( void * pvParameters );
void vLightLED2( void * pvParameters );
void vLightLED3( void * pvParameters );

#endif /* LED_TASKS_H_ */