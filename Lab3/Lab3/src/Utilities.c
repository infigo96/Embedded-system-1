/*
 * Utilities.c
 *
 * Created: 2018-09-17 17:17:32
 *  Author: Albert
 */ 
#include "Utilities.h"
//Divides total number of seconds into seconds, minuts and hours. Then, outputs it as a string (with cursermovment)
void Convert_Sec_To_String(char *timestring){
	char seconds[4], minuts[5], hours[5];
	char output[15];


	sprintf(hours, "%dh ", (time / 3600) % 24);
	sprintf(minuts, "%dm ", (time / 60) % 60);
	sprintf(seconds, "%ds", time % 60);
	
	strcat(output,hours);
	strcat(output,minuts);
	strcat(output,seconds);
	strcat(output, 1);
	strcpy(timestring, output);
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
