/*
 * GPIO_INTERFACE.h
 *
 * Created: 2018-09-20 17:34:46
 *  Author: Albert
 */ 


#ifndef GPIO_INTERFACE_H_
#define GPIO_INTERFACE_H_

# include "board.h"
# include "compiler.h"
#include "tc.h"
#include "avr32\uc3a0512.h"
#define MAXPIN 32
#define BUTTON_PORT (GPIO_PUSH_BUTTON_0 / MAXPIN)
#define BUTTON_PIN (1 << (GPIO_PUSH_BUTTON_0 & (MAXPIN - 1)))

#define JOYSTICK_PORT (GPIO_JOYSTICK_DOWN / MAXPIN)
#define JOYSTICK_PIN (1 << (GPIO_JOYSTICK_DOWN & (MAXPIN - 1)))

void GPIO_init();



#endif /* GPIO_INTERFACE_H_ */