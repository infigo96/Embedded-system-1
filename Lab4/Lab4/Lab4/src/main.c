/********************************************************
 Name          : main.c
 Author        : Marcus Jansson
 Copyright     : GPL
 Description   : EVK1100 template
 **********************************************************/

// Include Files
#include "board.h"
#include "adc.h"
#include "display_init.h"
#include <stdio.h>
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
	int pot_value;
	int brightness_LED5;

	// Configure the ADC module and enable the potentiometer channel
	adc_configure(&AVR32_ADC);
	adc_enable(&AVR32_ADC, ADC_POTENTIOMETER_CHANNEL);
   
	// Start a ADC sampling of all active channels
	adc_start(&AVR32_ADC);

	// Get the potentiometer value
	pot_value = adc_get_value(&AVR32_ADC, ADC_POTENTIOMETER_CHANNEL);

	// Convert the potentiometer value to a value btwn 0-255
	brightness_LED5 = pot_value * 255 / 1024;

	// Set the intensity of the LED
	LED_Set_Intensity(LED5, brightness_LED5);
	
	display_init();
	char text[25] = "Hello world";
	while(true)
	{
		dip204_set_cursor_position(1,1);
		sprintf(text,"%d",pot_value);
		dip204_printf_string(text);
		delay_s(1);
	}

	// Never return from main
	while(true);
}
