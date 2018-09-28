#ifndef PWM_LED_H_
#define PWM_LED_H_
#include "stdio.h"
#define itp32 (unsigned int*) //Used to convert to addresses
#define WRAP_VALUE 0x18 //15 results in 1 ms duty cycle on 32KHz clock. WRAP_VALUE is a 32bit number.

// All LEDs are on Port B
// PB19 - PB22 & PB27 - PB30, Found on page 46
#define GPIO_ADDRESS 0xFFFF1000  // Found on page 38
#define GPIO_LED_PORT 0x0100 // GPIO_PORT1 (B) = 0x0100 offset
#define GPIO_LED_CONTROL (GPIO_ADDRESS + GPIO_LED_PORT)

#define GPIO_LED0_PIN 27
#define GPIO_LED1_PIN 28
#define GPIO_LED2_PIN 29
#define GPIO_LED3_PIN 30

#define GPIO_LED4_PIN 19
#define GPIO_LED5_PIN 20
#define GPIO_LED6_PIN 21
#define GPIO_LED7_PIN 22

#define SET 0x04 // SET REGISTER OFFSET
#define CLR 0x08 // CLEAR REGISTER OFFSET
#define TOG 0x0C // TOGGLE REGISTER OFFSET
#define GPIO_GPER 0x00 // GENERALL PURPOSE ENABLE REGISTER
#define GPIO_ODER 0x40 // OUTPUT DRIVE ENABLE REGISTER
#define GPIO_OVR 0x50 // OUTPUT VALUE REGISTER 

#define PM_ADDRESS 0xFFFF0C00 //address to the Power Manager (PM)
#define O32KZCR 0x0030 // PM 32KHz Oscillator control register
#define MCCTRL 0x0000 // Main clock control
#define O0CTRL 0x0028 // Oscillator 0 control

// RTC clock is on PC
#define RTC_ADDRESS 0xFFFF0D00
//RTC UI
#define RTC_CONTROL 0x00
#define RTC_VALUE 0x04
#define RTC_TOP 0x08
#define RTC_INTERUPT_ENABLE 0x10
#define RTC_INTERUPT_STATUS 0x1C
#define RTC_INTERUPT_CLEAR 0x20

void initLED(int PIN);
void initRTC(void);
void initMAINCLK(void);
int readRTC(void);
int PWM(float duty_cycle);
#endif /* PWM_LED_H_ */