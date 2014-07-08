#define F_CPU 14745600
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <math.h>
#include "lcd.h"
#include "accelerometer.h"

int main(void)
{
	
	unsigned char x,y,z;     //Initialise variables
	acc_init_devices();      //Initialise the Ports for LCD & ADC
	lcd_cursor(2,8);
	
	while(1)
	{
		acc_process();    //Read the ADC values of the
                  //Accelerometer connected to the ADC pins
		x=acc_get_x();             //Get X Co-ordinate values
		y=acc_get_y();             //Get Y Co-ordinate values
		z=acc_get_z();             //Get Z Co-ordinate values
		lcd_print(2,1,x,3);        //Display the X value in LCD  
lcd_print(2,6,y,3);        //Display the Y value in LCD
lcd_print(2,11,z,3);       //Display the Z value in LCD
 
	}		
return 0;
}