/********************************************************
 Name          : main.c
 Author        : Marcus Jansson
 Copyright     : GPL
 Description   : EVK1100 template
 **********************************************************/
#define SAMPELS 10
// Include Files
#include "board.h"
#include "adc.h"
#include "display_init.h"
#include <stdio.h>
#include "pwm.h"
void mdelay(int ms);
void mdelay(int ms)		//Delay function
{
	long volatile cycles = 10*ms;
	while (cycles != 0)
	{
		cycles--;
	}
}
int main(void)
{
	volatile int pot_value; //Potentiometer
	volatile int lig_value; //Light
	volatile int tem_value; //Temperature
	int brightness_LED5;

	// Configure the ADC module and enable the potentiometer channel
	adc_configure(&AVR32_ADC);
	adc_enable(&AVR32_ADC, ADC_POTENTIOMETER_CHANNEL);
    adc_enable(&AVR32_ADC, ADC_LIGHT_CHANNEL);
	adc_enable(&AVR32_ADC, ADC_TEMPERATURE_CHANNEL);
	

	



	delay_init();
	display_init();
	char text[25] = "Hello world";
	LED_Set_Intensity(LED7, 0.9*255);	//I use this to get the waveform init done and one led set.
	
	avr32_pwm_channel_t hej; 
	hej.cprd = 0x000000FF;		//Set up some predefined like in LED_Set_Intensity
	hej.cmr = (AVR32_PWM_CPRE_MCK << AVR32_PWM_CPRE_OFFSET) &	~(AVR32_PWM_CALG_MASK |	AVR32_PWM_CPOL_MASK | AVR32_PWM_CPD_MASK);
	
	hej.cdty = 0.6*255;		//Set my intensity.
	AVR32_GPIO.port[1].pmr0c = 1<<20;	//Map the port to the PWM channel
	AVR32_GPIO.port[1].pmr1c = 1<<20;
	AVR32_GPIO.port[1].gperc = 1<<20;
	pwm_channel_init(1, &hej);
	pwm_start_channels(1<<1);
	
	hej.cdty = 0.3*255;
	AVR32_GPIO.port[1].pmr0s = 1<<27;	//Had to use LED 0 or 1 since there is only 4 green on PWM LED 0,1, 5,7. LED 4 and 6 are red and on the same physical as 5 and 7
	AVR32_GPIO.port[1].pmr1c = 1<<27;
	AVR32_GPIO.port[1].gperc = 1<<27;
	pwm_channel_init(4, &hej);
	pwm_start_channels(1<<4);
	
	while(1)
	{
		lig_value = 0;
		tem_value = 0;
		for (int i=0;i<SAMPELS;i++)
		{
			adc_start(&AVR32_ADC); // Start a ADC sampling of all active channels
			lig_value += adc_get_value(&AVR32_ADC, ADC_LIGHT_CHANNEL); // Get the light sensor value
			tem_value += adc_get_value(&AVR32_ADC, ADC_TEMPERATURE_CHANNEL); // Get the temperature sensor value
		}
		// Get the average
		lig_value /= SAMPELS;
		tem_value /= SAMPELS;
		// Get the potentiometer value
		pot_value = adc_get_value(&AVR32_ADC, ADC_POTENTIOMETER_CHANNEL);

		//// Convert the potentiometer value to a value btwn 0-255
		brightness_LED5 = pot_value * 255 / 1024;
		//
		//// Set the intensity of the LED
		
		//Clear the display
		//dip204_clear_display();
		
		//Write first row
		dip204_set_cursor_position(1,1);
		sprintf(text,"%04d",pot_value);
		dip204_printf_string(text);
		
		//Write second row
		dip204_set_cursor_position(1,2);
		sprintf(text,"%04d",lig_value);
		dip204_printf_string(text);


		//Write third row
		dip204_set_cursor_position(1,3);
		sprintf(text,"%04d",tem_value);
		dip204_printf_string(text);
		delay_ms(1000);
	}

	// Never return from main
	while(true);
}
