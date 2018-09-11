/*
 * LED.c
 *
 * Created: 2018-09-11 15:01:34
 *  Author: Daniel
 */ 
 #include "LED.h"

 void initLEDs(int LEDInit_decValue, int *PORTs_Arr, int* BITVALUEs_arr)
 {
	 int ledArr[8] = {1,2,4,8,16,32,64,128};
	 volatile avr32_gpio_port_t *led0_port, *led1_port, *led2_port, *led3_port, *led4_port, *led5_port, *led6_port, *led7_port;
	 
	 //----------------------------------------LED0-----------------------------------------------------//
	 if((LEDInit_decValue & ledArr[0]) != 0)
	 {
		 //Initiate LED0
		 led0_port = &AVR32_GPIO.port[PORTs_Arr[0]];
		 led0_port->gpers = BITVALUEs_arr[0];
		 led0_port->ovrs = BITVALUEs_arr[0];
		 led0_port->oders = BITVALUEs_arr[0];
	 }
	 //----------------------------------------LED1-----------------------------------------------------//
	 if((LEDInit_decValue & ledArr[1]) != 0)
	 {
		 //Initiate LED1
		 led1_port = &AVR32_GPIO.port[PORTs_Arr[1]];
		 led1_port->gpers = BITVALUEs_arr[1];
		 led1_port->ovrs = BITVALUEs_arr[1];
		 led1_port->oders = BITVALUEs_arr[1];
	 }
	 //----------------------------------------LED2-----------------------------------------------------//
	 if((LEDInit_decValue & ledArr[2]) != 0)
	 {
		 //Initiate LED2
		 led2_port = &AVR32_GPIO.port[PORTs_Arr[2]];
		 led2_port->gpers = BITVALUEs_arr[2];
		 led2_port->ovrs = BITVALUEs_arr[2];
		 led2_port->oders = BITVALUEs_arr[2];
	 }
	 //----------------------------------------LED3-----------------------------------------------------//
	 if((LEDInit_decValue & ledArr[3]) != 0)
	 {
		 //Initiate LED3
		 led3_port = &AVR32_GPIO.port[PORTs_Arr[3]];
		 led3_port->gpers = BITVALUEs_arr[3];
		 led3_port->ovrs = BITVALUEs_arr[3];
		 led3_port->oders = BITVALUEs_arr[3];
	 }
	 //----------------------------------------LED4-----------------------------------------------------//
	 if((LEDInit_decValue & ledArr[4]) != 0)
	 {
		 //Initiate LED4
		 led4_port = &AVR32_GPIO.port[PORTs_Arr[4]];
		 led4_port->gpers = BITVALUEs_arr[4];
		 led4_port->ovrs = BITVALUEs_arr[4];
		 led4_port->oders = BITVALUEs_arr[4];
	 }
	 //----------------------------------------LED5-----------------------------------------------------//
	 if((LEDInit_decValue & ledArr[5]) != 0)
	 {
		 //Initiate LED5
		 led5_port = &AVR32_GPIO.port[PORTs_Arr[5]];
		 led5_port->gpers = BITVALUEs_arr[5];
		 led5_port->ovrs = BITVALUEs_arr[5];
		 led5_port->oders = BITVALUEs_arr[5];
	 }
	 //----------------------------------------LED6-----------------------------------------------------//
	 if((LEDInit_decValue & ledArr[6]) != 0)
	 {
		 //Initiate LED6
		 led6_port = &AVR32_GPIO.port[PORTs_Arr[6]];
		 led6_port->gpers = BITVALUEs_arr[6];
		 led6_port->ovrs = BITVALUEs_arr[6];
		 led6_port->oders = BITVALUEs_arr[6];
	 }
	 //----------------------------------------LED7-----------------------------------------------------//
	 if((LEDInit_decValue & ledArr[7]) != 0)
	 {
		 //Initiate LED7
		 led7_port = &AVR32_GPIO.port[PORTs_Arr[7]];
		 led7_port->gpers = BITVALUEs_arr[7];
		 led7_port->ovrs = BITVALUEs_arr[7];
		 led7_port->oders = BITVALUEs_arr[7];
	 }
}

void lightLED(int LEDnr, int *PORTs_Arr, int* BITVALUEs_arr)
{
	 int ledArr[8] = {1,2,4,8,16,32,64,128};
	 volatile avr32_gpio_port_t *led0_port, *led1_port, *led2_port, *led3_port, *led4_port, *led5_port, *led6_port, *led7_port;
	 
	 //----------------------------------------LED0-----------------------------------------------------//
	 if((LEDnr & ledArr[0]) != 0)
	 {
	 //Initiate LED0
		 led0_port = &AVR32_GPIO.port[PORTs_Arr[0]];
		 led0_port->ovrc = BITVALUEs_arr[0];
	 }
	 //----------------------------------------LED1-----------------------------------------------------//
	 if((LEDnr & ledArr[1]) != 0)
	 {
		 //Initiate LED1
		 led1_port = &AVR32_GPIO.port[PORTs_Arr[1]];
		 led1_port->ovrc = BITVALUEs_arr[1];
	 }
	 //----------------------------------------LED2-----------------------------------------------------//
	 if((LEDnr & ledArr[2]) != 0)
	 {
		 //Initiate LED2
		 led2_port = &AVR32_GPIO.port[PORTs_Arr[2]];
		 led2_port->ovrc = BITVALUEs_arr[2];
	 }
	 //----------------------------------------LED3-----------------------------------------------------//
	 if((LEDnr & ledArr[3]) != 0)
	 {
		 //Initiate LED3
		 led3_port = &AVR32_GPIO.port[PORTs_Arr[3]];
		 led3_port->ovrc = BITVALUEs_arr[3];
	 }
	 //----------------------------------------LED4-----------------------------------------------------//
	 if((LEDnr & ledArr[4]) != 0)
	 {
		 //Initiate LED4
		 led4_port = &AVR32_GPIO.port[PORTs_Arr[4]];
		 led4_port->ovrc = BITVALUEs_arr[4];
	 }
	 //----------------------------------------LED5-----------------------------------------------------//
	 if((LEDnr & ledArr[5]) != 0)
	 {
		 //Initiate LED5
		 led5_port = &AVR32_GPIO.port[PORTs_Arr[5]];
		 led5_port->ovrc = BITVALUEs_arr[5];
	 }
	 //----------------------------------------LED6-----------------------------------------------------//
	 if((LEDnr & ledArr[6]) != 0)
	 {
		 //Initiate LED6
		 led6_port = &AVR32_GPIO.port[PORTs_Arr[6]];
		 led6_port->ovrc = BITVALUEs_arr[6];
	 }
	 //----------------------------------------LED7-----------------------------------------------------//
	 if((LEDnr & ledArr[7]) != 0)
	 {
		 //Initiate LED7
		 led7_port = &AVR32_GPIO.port[PORTs_Arr[7]];
		 led7_port->ovrc = BITVALUEs_arr[7];
	 }
}


void closeLED(int LEDnr, int *PORTs_Arr, int* BITVALUEs_arr)
{
	int ledArr[8] = {1,2,4,8,16,32,64,128};
	volatile avr32_gpio_port_t *led0_port, *led1_port, *led2_port, *led3_port, *led4_port, *led5_port, *led6_port, *led7_port;
	
	//----------------------------------------LED0-----------------------------------------------------//
	if((LEDnr & ledArr[0]) != 0)
	{
		//Initiate LED0
		led0_port = &AVR32_GPIO.port[PORTs_Arr[0]];
		led0_port->ovrs = BITVALUEs_arr[0];
	}
	//----------------------------------------LED1-----------------------------------------------------//
	if((LEDnr & ledArr[1]) != 0)
	{
		//Initiate LED1
		led1_port = &AVR32_GPIO.port[PORTs_Arr[1]];
		led1_port->ovrs = BITVALUEs_arr[1];
	}
	//----------------------------------------LED2-----------------------------------------------------//
	if((LEDnr & ledArr[2]) != 0)
	{
		//Initiate LED2
		led2_port = &AVR32_GPIO.port[PORTs_Arr[2]];
		led2_port->ovrs = BITVALUEs_arr[2];
	}
	//----------------------------------------LED3-----------------------------------------------------//
	if((LEDnr & ledArr[3]) != 0)
	{
		//Initiate LED3
		led3_port = &AVR32_GPIO.port[PORTs_Arr[3]];
		led3_port->ovrs = BITVALUEs_arr[3];
	}
	//----------------------------------------LED4-----------------------------------------------------//
	if((LEDnr & ledArr[4]) != 0)
	{
		//Initiate LED4
		led4_port = &AVR32_GPIO.port[PORTs_Arr[4]];
		led4_port->ovrs = BITVALUEs_arr[4];
	}
	//----------------------------------------LED5-----------------------------------------------------//
	if((LEDnr & ledArr[5]) != 0)
	{
		//Initiate LED5
		led5_port = &AVR32_GPIO.port[PORTs_Arr[5]];
		led5_port->ovrs = BITVALUEs_arr[5];
	}
	//----------------------------------------LED6-----------------------------------------------------//
	if((LEDnr & ledArr[6]) != 0)
	{
		//Initiate LED6
		led6_port = &AVR32_GPIO.port[PORTs_Arr[6]];
		led6_port->ovrs = BITVALUEs_arr[6];
	}
	//----------------------------------------LED7-----------------------------------------------------//
	if((LEDnr & ledArr[7]) != 0)
	{
		//Initiate LED7
		led7_port = &AVR32_GPIO.port[PORTs_Arr[7]];
		led7_port->ovrs = BITVALUEs_arr[7];
	}
}
