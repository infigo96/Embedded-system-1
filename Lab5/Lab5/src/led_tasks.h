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
#include "gpio.h"
#include "pm.h"
#include "usart.h"

# define LED_PORT ( LED0_GPIO / 32 )
// This is the bit position of the GPIO pin
# define LED0_PIN 27
# define LED1_PIN 28
# define LED2_PIN 29
void initLED();
void vBlinkLED1( void * pvParameters );
void vBlinkLED2( void * pvParameters );
void vBlinkLED3( void * pvParameters );


#endif /* LED_TASKS_H_ */