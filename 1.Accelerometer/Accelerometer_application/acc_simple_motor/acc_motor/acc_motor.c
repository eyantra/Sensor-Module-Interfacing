#define F_CPU 14745600
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <math.h>
#include "lcd.h"
#include "accelerometer.h"


unsigned char l=0,r=0,b=0,f=0,s=0;

void motion_pin_config (void)
{
	DDRA = DDRA | 0x0f;
	PORTA = PORTA & 0xf0;
	DDRL = DDRL | 0x18;   //Setting PL3 and PL4 pins as output for PWM generation
	PORTL = PORTL | 0x18; //PL3 and PL4 pins are for velocity control using PWM.
}


// Function for robot velocity control



void forward (void) //both wheels forward
{
	PORTA=0x06;
	//motion_set(0x30);
}

void left(void)
{
	
	PORTA=0x05;
}
	

void right(void)
{
	
	PORTA=0x0A;
}



void back (void) //both wheels backward
{
	PORTA=0x09;
}

void stop (void)
{
	PORTA=0x00;
}


void acc_direction(void)
{
	if(x<90 && y>95 && z<107) r=1;
	if(x<90 && y<79 && z<107) l=1;
	if(x>100 && y<95 && z<107) b=1;
	if(x<75 && y<95 && z<107) f=1;
	if(x>80 && x<90 && y>85 && y<92 && z<110) s=1;
	//acc_call_motion();
	if(r==1)
	{
		lcd_cursor(2,10);
		lcd_string("right");//l=b=f=0;
		right();
	}
	if(l==1)
	{
		lcd_cursor(2,10);
		lcd_string("left  ");//r=b=f=0;
		left();
	}
	if(b==1)
	{
		lcd_cursor(2,10);
		lcd_string("back  ");//r=l=f=0;
		back();
		//_delay_ms(100);
	}
	if(f==1)
	{
		lcd_cursor(2,10);
		lcd_string("fwd   ");//r=l=b=0;
	forward();
		//_delay_ms(1000);
	}
	if(s==1)
	{
		lcd_cursor(2,10);
		lcd_string("steady   ");//l=b=f=0;
		stop();
		//_delay_ms(1000);
	
	}


} 

int main(void)
{
	
	acc_init_devices();			//initializes all the adc channels as input and output.
	while(1)
	{

		acc_process();			// gets the x y z values from the sensors and displays it on the lcd screen
		motion_pin_config();	// configure the dc motors i/o ports
		acc_direction();		// compares the obtained xyz values with the threshold and chooses the appropriate orientation.
		r=l=f=b=s=0;			//reset the flags responsible for direction decision
	
	
	}
	return 0;
}

