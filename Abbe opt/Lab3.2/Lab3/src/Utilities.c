/*
 * Utilities.c
 *
 * Created: 2018-09-17 17:17:32
 *  Author: Albert
 */ 
#include "Utilities.h"
#include "USART_driver.h"
//Divides total number of seconds into seconds, minuts and hours. Then, outputs it as a string (with ANSI cursor movement)
void Convert_Sec_To_String(char *timestring, int timeValue){
	char seconds[4], minuts[5], hours[5]; //Stores the seconds, minuts and hours as text
	char output[25]; //The combined output


	sprintf(hours, "%dh ", (timeValue / 3600) % 24);
	sprintf(minuts, "%dm ", (timeValue / 60) % 60);
	sprintf(seconds, "%ds", timeValue % 60);
	
	char esc_Seq[7];
    sprintf(esc_Seq, "%c[2J",0x1B);		//Empty the terminal window
	strcpy(output,esc_Seq);
	
	sprintf(esc_Seq, "%c[0;0H",0x1B);	//Move the cursor to the beginning of the row/column in the terminal
	strcat(output,esc_Seq);
	//Append the time
	strcat(output,hours);
	strcat(output,minuts);
	strcat(output,seconds);
	strcpy(timestring, output); //Copy to the inputted string
}

// Delay input milli sec.
void mdelay(int ms)
{
	// The multiplicator 1091 is the estimated number of cycles per mili sec based on the calculation 12000/11 (instead of 115,2/11).
	long volatile cycles = 1091*ms;
	while (cycles != 0)
	{
		cycles--;
	}
}

void SendTime(int time){
	char timeString[25]; //Stores the formated time
	int localTime = (int)(time/10);
	Convert_Sec_To_String(timeString, localTime);
	USART_putString(timeString);
}