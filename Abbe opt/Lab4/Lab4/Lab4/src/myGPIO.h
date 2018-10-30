/*
 * myGPIO.h
 *
 * Created: 2018-10-16 13:45:16
 *  Author: Albert
 */ 


#ifndef MYGPIO_H_
#define MYGPIO_H_
# include "board.h"
# include "gpio.h"
//The LED addresses
# define LED_PORT ( LED0_GPIO / 32 )
# define LED0_PIN 27
# define LED1_PIN 28
# define LED2_PIN 29

//The button addresses
#define BUTTON_PORT (GPIO_PUSH_BUTTON_0 >> 5)#define BUTTON0_PIN 1 << (GPIO_PUSH_BUTTON_0 & 0x1f)
#define BUTTON1_PIN 1 << (GPIO_PUSH_BUTTON_1 & 0x1f)
#define BUTTON2_PIN 1 << (GPIO_PUSH_BUTTON_2 & 0x1f)

void initLED();


#endif /* MYGPIO_H_ */