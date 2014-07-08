
#define F_CPU 14745600

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "lcd.h"

//Function to configure PIR port
void PIR(void)
{
	DDRL &= ~(1<<PL6);				//Setting the direction registers to make PL6 Pin as INPUT
	PORTL |= (1 << PL6);			//Setting the Port L PIN 6 as Floating
}

//Function to configure Bargraph LED port
void LED(void)
{
	DDRJ = 0xFF;					//Setting the direction registers to make all the PORT J pins as OUTPUT
	PORTJ = 0x00;					//Setting all the PINs of PORT J initially as LOGIC LOW
}

//Function to configure LCD port
void lcd_port_config (void)
{
	DDRC = DDRC | 0xF7; //all the LCD pin's direction set as output
	PORTC = PORTC & 0x80; // all the LCD pins are set to logic 0 except PORTC 7
}

//Function to Initialize PORTS
void port_init()
{
	lcd_port_config();
	LED();
	PIR();
}


void init_devices (void)
{
	cli(); //Clears the global interrupts
	port_init();
	sei();   //Enables the global interrupts
}

//Main Function
int scan_pir(void)
{

	unsigned char x;
	int flag=1;
	
	init_devices();			//Initialize all the devices
	lcd_set_4bit();			//Set the LCD in 4 bit mode
	
	while(1)
	{
		
		x = PINL & (1<< 6);			//Read the PL6 pin to read the PIR Sensor output
		lcd_init();					//Initialize the LCD
		if(x)						//Check if the PL6 pin is HIGH
		{
			PORTJ = 0xFF;			//Turn ON Bargraph LED
			
			
			
			if(flag ==1)			// A flag has been added to stop the screen from re-initializing every time in the while loop
			{
				lcd_init();
				lcd_cursor(1,1);
				
			}
			lcd_string("Human Detected");		//Display "Human Detected" on the LCD
			flag=0;
		}
		else
		{
			
			PORTJ = 0x00;			//Turn OFF Bargraph LED
			
			if(flag==0)
			{
				lcd_init();
				lcd_cursor(1,1);
			}
			lcd_string("Empty Space");			//Display "Empty Space" on the LCD
			flag=1;
		}
		
	}
}
