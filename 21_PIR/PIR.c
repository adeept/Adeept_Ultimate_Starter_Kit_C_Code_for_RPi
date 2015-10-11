/*
* File name   : PIR.c
* Description : detect the movement.
* Website     : www.adeept.com
* E-mail      : support@adeept.com
* Author      : Jason
* Date        : 2015/10/12
*/
#include <wiringPi.h>
#include <stdio.h>

#define PIR_OUT 0

int status = 1;

void myISR(void)
{
	status = !status;	
}

int main(void)
{
	if(wiringPiSetup() == -1){ //when initialize wiring failed,print message to screen
		printf("setup wiringPi failed !\n");
		return -1; 
	}
	
	pinMode(PIR_OUT, INPUT);

	pullUpDnControl(PIR_OUT, PUD_UP);
	
	if(wiringPiISR(ButtonPin, INT_EDGE_FALLING, myISR) < 0){
		printf("ISR setup error!\n");
		return -1;
	}

	while(1){
		if(status == 0){
			printf("detected!\n");	
		}else{
			printf("No!\n");	
		}
	}

	return 0;
}
