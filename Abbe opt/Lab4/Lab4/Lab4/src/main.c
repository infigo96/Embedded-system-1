/********************************************************
 Name          : main.c
 Author        : Marcus Jansson
 Copyright     : GPL
 Description   : EVK1100 template
 **********************************************************/
#define SAMPELS 10
// Include Files
#include "myGPIO.h"
#include "adc.h"
#include "display_init.h"
#include "Stopwatch.h"
#include <stdio.h>

void PWMLED_SET(int PIN,int intens){
	volatile avr32_pwm_t *pwm = &AVR32_PWM;
	volatile avr32_pwm_channel_t *pwm_ch = &(pwm->channel[0]);
	pwm_ch->cdty=intens;
	if (pwm_ch->ccnt>pwm_ch->cdty)
	{
		AVR32_GPIO.port[LED_PORT].ovrs = (1 << PIN);
	}
	else
	{
		AVR32_GPIO.port[LED_PORT].ovrc = (1 << PIN);
	}
}
void PWMLED_Set_Channel(int PIN,int ch){
	volatile avr32_pwm_t *pwm = &AVR32_PWM;
	volatile avr32_pwm_channel_t *pwm_ch = &(pwm->channel[ch]);
	if (pwm_ch->ccnt>pwm_ch->cdty)
	{
		AVR32_GPIO.port[LED_PORT].ovrs = (1 << PIN);
	}
	else
	{
		AVR32_GPIO.port[LED_PORT].ovrc = (1 << PIN);
	}
}
int main(void)
{	
	
	unsigned int channel0 = 0,channel1 = 1,channel2 = 2;
	
	PWM_init();
	PWM_start(channel0);
	initLED();
	
	while(1)
	{
		PWMLED_SET(LED0_PIN,10);
		PWMLED_SET(LED1_PIN,60);
		PWMLED_SET(LED2_PIN,100);
		//a = pwm->channel[channel].ccnt;
		//value = AVR32_GPIO.port[1].pvr & PWM_PIN;
		//b = pwm->channel[channel].cdty;
		//c = pwm->channel[channel].cmr;
		//d = pwm->channel[channel].cprd;
		//e = pwm->channel[channel].cupd;
		//value = readPIN();
		
		//value = tc_read_sr(tc, channel);
		//// Set the intensity of the LED
		//LED_Set_Intensity(LED5, 100);
	}

	// Never return from main
	while(true);
}
