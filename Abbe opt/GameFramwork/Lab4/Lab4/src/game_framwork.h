/*
 * game_framwork.h
 *
 * Created: 2018-09-28 18:52:38
 *  Author: Albert
 */ 


#ifndef GAME_FRAMWORK_H_
#define GAME_FRAMWORK_H_
#include <stdio.h>
#include <time.h>
#include "display_init.h"
#include "board.h"
#include "adc.h"
#include "tc.h"

//Select game to load
#include "pong.h"
//GPIO
#define MAXPIN 32
#define BUTTON_PORT (GPIO_PUSH_BUTTON_0 / MAXPIN)
#define BUTTON_0_PIN (1 << (GPIO_PUSH_BUTTON_0 & (MAXPIN - 1)))
#define BUTTON_1_PIN (1 << (GPIO_PUSH_BUTTON_1 & (MAXPIN - 1)))
#define BUTTON_2_PIN (1 << (GPIO_PUSH_BUTTON_2 & (MAXPIN - 1)))

#define JOYSTICK_PORT (GPIO_JOYSTICK_DOWN / MAXPIN)
#define JOYSTICK_DOWN_PIN (1 << (GPIO_JOYSTICK_DOWN & (MAXPIN - 1)))
#define JOYSTICK_RIGHT_PIN (1 << (GPIO_JOYSTICK_RIGHT & (MAXPIN - 1)))
#define JOYSTICK_LEFT_PIN (1 << (GPIO_JOYSTICK_LEFT & (MAXPIN - 1)))
#define JOYSTICK_UP_PIN (1 << (GPIO_JOYSTICK_UP & (MAXPIN - 1)))
#define JOYSTICK_PUSH_PIN (1 << (GPIO_JOYSTICK_PUSH & (MAXPIN - 1)))

unsigned long int global_time;
typedef enum DIRECTIONS{NONE, LEFT, RIGHT, UP ,DOWN, PUSH} DIRECTIONS;
	
void framework_init();
void initCLK(volatile avr32_tc_t *tc);
void increment_tick(); //Called every 0.1 second
DIRECTIONS getJoystickInput();
int getButtonInput(int button);
void drawAtPos(char c, int x, int y);
void delay(int t);
//Inputs


#endif /* GAME_FRAMWORK_H_ */