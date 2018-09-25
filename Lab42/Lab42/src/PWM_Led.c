#include "PWM_Led.h"
// Setup a specific LED based on PIN
void initLED(int PIN)
{
	*(itp32(GPIO_LED_CONTROL + GPIO_GPER ))|= (0x01 << PIN); // Enable the general purpose register
	*(itp32(GPIO_LED_CONTROL + GPIO_ODER ))|= (0x01 << PIN); // Enable the output drive
	*(itp32(GPIO_LED_CONTROL + GPIO_OVR )) &= ~(0x01 << PIN); // Set the output bit to 0
	
}
//Setup the real time clock (RTC)
void initRTC(void)
{
	//Start the 32KHz crystal
	*(itp32(PM_ADDRESS + O32KZCR ))|= (0x01 << 0); //OSC32EN
	*(itp32(PM_ADDRESS + O32KZCR ))|= (0x01 << 8); //Oscillator Mode
 	*(itp32(PM_ADDRESS + O32KZCR ))|= (0x02 << 16); //Oscillator Startup Time
	 
	 //Enable the real time clock
	*(itp32(RTC_ADDRESS + RTC_CONTROL ))|= (0x01 << 0); // RTC enable
	*(itp32(RTC_ADDRESS + RTC_CONTROL ))|= (0x01 << 3); // CLK32, use the 32KHz oscillator as clock source
	*(itp32(RTC_ADDRESS + RTC_CONTROL ))|= (0x01 << 16); // CLKEN enable
	*(itp32(RTC_ADDRESS + RTC_TOP )) = WRAP_VALUE; //Value to wrap around on
	*(itp32(RTC_ADDRESS + RTC_VALUE)) = 0x00;
}
// Returns the reading of the RTC clock
int readRTC(void)
{
	return *(itp32(RTC_ADDRESS + RTC_VALUE )); 
}
//Generates the PWM
int PWM(float duty_cycle)
{
	if(readRTC() > WRAP_VALUE * duty_cycle)
		return 1;
	else
		return 0;
}
//Set the Main clock to Oscillator 0
void initMAINCLK()
{
	*(itp32(PM_ADDRESS + O0CTRL)) = 0x604; // MODE Select = 4' // STARTUP time Select = 6'
	*(itp32(PM_ADDRESS + MCCTRL)) |= (0x01 << 2); // Enable OSC0'
	*(itp32(PM_ADDRESS + MCCTRL)) |= (0x01 << 0); // Select Main clock to OSC0 = 1'
}