#ifndef PWM_LED_H_
#define PWM_LED_H_

// All LEDs are on PB
// PB19 - PB22 & PB27 - PB30
// GPIO_PORT1 0x0100
#define GPIO_ADRESS 0xFFFF1000  // Found on page 38
#define GPIO_LED_PORT 0x0100
#define GPIO_LED_CONTROL (GPIO_ADRESS + LED_PORT)

#define GPIO_LED0_PIN 27
#define GPIO_LED1_PIN 28
#define GPIO_LED2_PIN 29
#define GPIO_LED3_PIN 30

#define GPIO_LED4_PIN 19
#define GPIO_LED5_PIN 20
#define GPIO_LED6_PIN 21
#define GPIO_LED7_PIN 22

#define GPIO_Set 0x04 // SET ENABLE REGISTER OFFSET
#define GPIO_Clr 0x08 // CLEAR ENABLE REGISTER OFFSET
#define GPIO_Tog 0x0C // TOGGLE ENABLE REGISTER OFFSET




#endif /* PWM_LED_H_ */