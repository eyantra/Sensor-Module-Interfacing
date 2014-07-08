/*
 * accelerometer1.c
 *
 * Created: 6/9/2014 12:59:48 PM
 *  Author: ERTS Lab
 */ 


#define F_CPU 14745600
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <math.h>
#include "lcd.h"
unsigned char ADC_Conversion(unsigned char);
unsigned char ADC_Value;
unsigned char l=0,r=0,b=0,f=0,s=0,x,y,z, adc_reading;

void lcd_port_config (void)
{
	DDRC = DDRC | 0xF7; 		//all the LCD pin's direction set as output
	PORTC = PORTC & 0x80; 		// all the LCD pins are set to logic 0 except PORTC 7
}


void adc_pin_config (void)
{
	DDRF = 0x00; 			//set PORTF direction as input
	PORTF = 0x00;			//set PORTF pins floating
	DDRK = 0x00; 			//set PORTK direction as input
	PORTK = 0x00; 			//set PORTK pins floating
}

void adc_init()
{
	ADCSRA = 0x00;
	ADCSRB = 0x00;			//MUX5 = 0
	ADMUX = 0x20;			//Vref=5V external --- ADLAR=1 --- MUX4:0 = 0000
	ACSR = 0x80;
	ADCSRA = 0x86;			//ADEN=1 --- ADIE=1 --- ADPS2:0 = 1 1 0
}


unsigned char ADC_Conversion(unsigned char Ch)
{
	unsigned char a;
	if(Ch>7)
	{
		ADCSRB = 0x08;
	}
	Ch = Ch & 0x07;
	ADMUX= 0x20| Ch;
	ADCSRA = ADCSRA | 0x40;		//Set start conversion bit
	while((ADCSRA&0x10)==0);	//Wait for ADC conversion to complete
	a=ADCH;
	ADCSRA = ADCSRA|0x10; 		//clear ADIF (ADC Interrupt Flag) by writing 1 to it
	ADCSRB = 0x00;
	return a;
}



void acc(void)
{
	
	x = ADC_Conversion(14);
	y= 	ADC_Conversion(15);	
	z= ADC_Conversion(11);
	lcd_cursor(1,1);
	lcd_string("X = ");
	lcd_print(1,4,x,3);
	lcd_cursor(1,7);
	lcd_string(" y = ");
	lcd_print(1,11,y,3);
	lcd_cursor(2,1);
	lcd_string("z = ");
	lcd_print(2,4,z,3);

}


void port_init()
{
	
	lcd_port_config();
	adc_pin_config();
}

void acc_process(void)
{
	if(x<90 && y>95 && z<107)					r=1;
	if(x<90 && y<79 && z<107)					l=1;
	if(x>100 && y<95 && z<107)					b=1;
	if(x<75 && y<95 && z<107)					f=1;
	if(x>80 && x<90 && y>85 && y<92 && z<110)	s=1;
	//acc_call_motion();
	if(r==1)
	{
		lcd_cursor(2,10);
		lcd_string("right");
	}
	if(l==1)
	{
		lcd_cursor(2,10);
		lcd_string("left  ");
	}
	if(b==1)
	{
		lcd_cursor(2,10);
		lcd_string("back  ");
	}
	if(f==1)
	{
		lcd_cursor(2,10);
		lcd_string("fwd   ");
	}
	if(s==1)
	{
		lcd_cursor(2,10);
		lcd_string("steady   ");
	}


} 

void acc_init_devices()		// this function initializes the lcd and adc I/O ports
{
	cli();
	port_init();
	lcd_init();
	adc_init();				//initializes all the adc channels as input and output.
	sei();
}


int main(void)
{
	
	acc_init_devices();
	while(1)
	{

		acc();				// The function is used to extract the adc values of x, y, and Z coordinates
		acc_process();		// This function actually, compares the obtained values with a threshold and updates necessary flags for further actions to take place
	
		r=l=f=b=s=0;		// reset all the flags 
	}
	return 0;
}

