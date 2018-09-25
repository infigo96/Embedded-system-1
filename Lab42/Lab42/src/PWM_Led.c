#include "PWM_Led.h"

void initLED(int PIN)
{
	
	*(itp32(GPIO_LED_CONTROL + GPIO_GPER ))|= (0x01 << PIN);
	*(itp32(GPIO_LED_CONTROL + GPIO_ODER ))|= (0x01 << PIN);
	*(itp32(GPIO_LED_CONTROL + GPIO_OVR ))|= ~(0x01 << PIN);
	
}

void initRTC(void)
{
	//Start the 32KHz crystal
	*(itp32(PM_ADRESS + O32KZCR ))|= (0x01 << 0); //OSC32EN
	*(itp32(PM_ADRESS + O32KZCR ))|= (0x01 << 8); //Oscillator Mode
 	*(itp32(PM_ADRESS + O32KZCR ))|= (0x02 << 16); //Oscillator Startup Time
	 
	 //Enable the real time clock
	*(itp32(RTC_ADRESS + RTC_CONTROL ))|= (0x01 << 0); // RTC enable
	*(itp32(RTC_ADRESS + RTC_CONTROL ))|= (0x01 << 3); // CLK32, use the 32KHz oscillator as clock source
	//*(itp32(RTC_ADRESS + RTC_CONTROL ))|= (0x01 << 16); // CLKEN enable
	*(itp32(RTC_ADRESS + RTC_TOP )) = WRAP_VALUE; //Value to wrap around on
}
int readRTC(void)
{
	return *(itp32(RTC_ADRESS + RTC_VALUE ));
}
//Generated
int PWM(float duty_cycle)
{
	if(readRTC() > WRAP_VALUE * duty_cycle)
		return 0;
	else
		return 1;
}
//Set the Main clock to Oscillator 0
void initMAINCLK()
{
	//*(itp32(PM_ADRESS + O0CTRL)) |= (0x04 << 0); // MODE Select = 4'
	//*(itp32(PM_ADRESS + O0CTRL)) |= (0x06 << 8); // STARTUP time Select = 6'
	//*(itp32(PM_ADRESS + MCCTRL)) |= (0x01 << 2); // Enable OSC0'
	//*(itp32(PM_ADRESS + MCCTRL)) |= (0x01 << 0); // Select Main clock to OSC0 = 1'
	
	*(itp32(PM_ADRESS + O0CTRL)) = 0x604; // MODE Select = 4' // STARTUP time Select = 6'
	//*(itp32(PM_ADRESS + MCCTRL)) = 0x05; // Enable OSC0' // Select Main clock to OSC0 = 1'
	volatile unsigned int* foo = itp32(PM_ADRESS + MCCTRL);
	*(itp32(PM_ADRESS + MCCTRL)) |= (0x01 << 2); // Enable OSC0'
	*(itp32(PM_ADRESS + MCCTRL)) |= (0x01 << 0); // Select Main clock to OSC0 = 1'
}