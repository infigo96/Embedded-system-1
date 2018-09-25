/*
 * LED.h
 *
 * Created: 2018-09-11 15:02:22
 *  Author: Daniel
 */ 


#ifndef LED_H_
#define LED_H_
#define GPIO_MAX_PIN_NUMBER 32

#include "avr32\uc3a0512.h"

void initLEDs(int LEDInit_decValue, int *PORTs_Arr, int* BITVALUEs_arr);
void lightLED(int LEDnr, int *PORTs_Arr, int* BITVALUEs_arr);
void closeLED(int LEDnr, int *PORTs_Arr, int* BITVALUEs_arr);


#endif /* LED_H_ */