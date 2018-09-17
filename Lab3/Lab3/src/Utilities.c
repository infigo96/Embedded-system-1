/*
 * Utilities.c
 *
 * Created: 2018-09-17 17:17:32
 *  Author: Albert
 */ 
#include "Utilities.h"
//Divides total number of seconds into seconds, minuts and hours. Then, outputs it as a string
void Convert_Sec_To_String(char *timestring){
	char seconds[4], minuts[5], hours[5];
	char output[12];


	sprintf(hours, "%dh ", (time / 3600) % 24);
	sprintf(minuts, "%dm ", (time / 60) % 60);
	sprintf(seconds, "%ds", time % 60);
	
	strcat(output,hours);
	strcat(output,minuts);
	strcat(output,seconds);
	strcpy(timestring, output);
}