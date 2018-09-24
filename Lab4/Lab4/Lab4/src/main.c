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
	

	


	//// Convert the potentiometer value to a value btwn 0-255
	//brightness_LED5 = pot_value * 255 / 1024;
//
	//// Set the intensity of the LED
	//LED_Set_Intensity(LED5, brightness_LED5);
	delay_init();
	display_init();
	char text[25] = "Hello world";
	
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
