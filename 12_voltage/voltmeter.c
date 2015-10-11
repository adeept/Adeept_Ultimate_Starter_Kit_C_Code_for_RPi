/*
* File name   : voltmeter.c
* Description : a simple voltmeter
* Website     : www.adeept.com
* E-mail      : support@adeept.com
* Author      : Jason
* Date        : 2015/06/04
*/
#include <stdio.h>
#include <stdlib.h>
#include <wiringPi.h>
#include <lcd.h>

typedef unsigned char uchar;
typedef unsigned int  uint;

#define     ADC_CS    12
#define     ADC_DIO   6
#define     ADC_CLK   13

uchar get_ADC_Result(void)
{
	//10:CH0
	//11:CH1
	uchar i;
	uchar dat1=0, dat2=0;

	digitalWrite(ADC_CS, 0);

	digitalWrite(ADC_CLK,0);
	digitalWrite(ADC_DIO,1);	delayMicroseconds(2);
	digitalWrite(ADC_CLK,1);	delayMicroseconds(2);
	digitalWrite(ADC_CLK,0);

	digitalWrite(ADC_DIO,1);    delayMicroseconds(2); //CH0 10
	digitalWrite(ADC_CLK,1);	delayMicroseconds(2);
	digitalWrite(ADC_CLK,0);

	digitalWrite(ADC_DIO,0);	delayMicroseconds(2);
	digitalWrite(ADC_CLK,1);
	digitalWrite(ADC_DIO,1);    delayMicroseconds(2);
	digitalWrite(ADC_CLK,0);	
	digitalWrite(ADC_DIO,1);    delayMicroseconds(2);

	for(i=0;i<8;i++)
	{
		digitalWrite(ADC_CLK,1);	delayMicroseconds(2);
		digitalWrite(ADC_CLK,0);    delayMicroseconds(2);

		pinMode(ADC_DIO, INPUT);
		dat1=dat1<<1 | digitalRead(ADC_DIO);
	}

	for(i=0;i<8;i++)
	{
		dat2 = dat2 | ((uchar)(digitalRead(ADC_DIO))<<i);
		digitalWrite(ADC_CLK,1); 	delayMicroseconds(2);
		digitalWrite(ADC_CLK,0);    delayMicroseconds(2);
	}

	digitalWrite(ADC_CS,1);

	pinMode(ADC_DIO, OUTPUT);

	return(dat1==dat2) ? dat1 : 0;
}

int main(void)
{
	int fd;
	uchar val;
	float vol;
	
	if(wiringPiSetup() == -1){
		printf("wiringPi setup failed !\n");
		exit(1);
	}
	
	pinMode(ADC_CS,  OUTPUT);
	pinMode(ADC_CLK, OUTPUT);
	
	fd = lcdInit(2,16,4, 5,4, 0,1,2,3,4,5,6,7);
	if (fd == -1){
		printf ("lcdInit failed\n") ;
		return 1;
	}
	delay(500);

	lcdPosition(fd, 0, 0); lcdPuts (fd, "   Voltmeter   ");
	delay(500);

	while(1){
		pinMode(ADC_DIO, OUTPUT);
		val = get_ADC_Result();
		printf("Current analog val: %d\n", val);

		vol = (5.0 / 255) * val;
		lcdPosition(fd, 0, 1);
		lcdPrintf(fd, "%s", "Voltage: ");
		lcdPrintf(fd, "%.2fV", vol);

		delay(500);
	}

	return 0;
}
