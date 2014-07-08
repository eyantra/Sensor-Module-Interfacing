#define F_CPU 147545600
#include<avr/io.h>
#include<util/delay.h>
#include "lcd.h"
#include "rfid.h"

void main(void)
{
	rf_init_devices();	//initializes the usart3, configures lcd ports and initializes the lcd,
	lcd_cursor(2,4);	//move the cursor to 2nd row 4th column
	_delay_ms(50);
	rf_display();		//this function is used to scan the rfid tag and is used to display the same on the lcd screen
	cli();
	lcd_wr_command(0x01);	//this code is used to clear the lcd screen
	
	return 0;
}
