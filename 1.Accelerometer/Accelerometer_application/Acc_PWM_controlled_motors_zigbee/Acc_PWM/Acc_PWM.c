#include<avr/io.h>
#include<avr/interrupt.h>
#include<util/delay.h>
#include "lcd.h"

unsigned char arr[100];
unsigned int j=0,i=0;
unsigned char data; //to store received data from UDR1
unsigned char xh, xl, yh, yl, h, f1,f2,b1,b2,r1,r2,l1,l2;


void timer5_init()
{
	TCCR5B = 0x00;	//Stop
	TCNT5H = 0xFF;	//Counter higher 8-bit value to which OCR5xH value is compared with
	TCNT5L = 0x01;	//Counter lower 8-bit value to which OCR5xH value is compared with
	OCR5AH = 0x00;	//Output compare register high value for Left Motor
	OCR5AL = 0xFF;	//Output compare register low value for Left Motor
	OCR5BH = 0x00;	//Output compare register high value for Right Motor
	OCR5BL = 0xFF;	//Output compare register low value for Right Motor
	OCR5CH = 0x00;	//Output compare register high value for Motor C1
	OCR5CL = 0xFF;	//Output compare register low value for Motor C1
	TCCR5A = 0xA9;	/*{COM5A1=1, COM5A0=0; COM5B1=1, COM5B0=0; COM5C1=1 COM5C0=0}
 					  For Overriding normal port functionality to OCRnA outputs.
				  	  {WGM51=0, WGM50=1} Along With WGM52 in TCCR5B for Selecting FAST PWM 8-bit Mode*/
	
	TCCR5B = 0x0B;	//WGM12=1; CS12=0, CS11=1, CS10=1 (Prescaler=64)
}

void velocity (unsigned char left_motor, unsigned char right_motor)
{
	OCR5AL = (unsigned char)left_motor;
	OCR5BL = (unsigned char)right_motor;
}

void uart0_init(void)
{
 UCSR0B = 0x00; //disable while setting baud rate
 UCSR0A = 0x00;
 UCSR0C = 0x06;
 UBRR0L = 0x5F; //set baud rate lo
 UBRR0H = 0x00; //set baud rate hi
 UCSR0B = 0x98;
}

void motion_pin_config (void)
{
	DDRA = DDRA | 0x0f;
	PORTA = PORTA & 0xf0;
	DDRL = DDRL | 0x18;   //Setting PL3 and PL4 pins as output for PWM generation
	PORTL = PORTL | 0x18; //PL3 and PL4 pins are for velocity control using PWM.
}

void lcd_port_config (void)
{
	DDRC = DDRC | 0xF7;
	PORTC = PORTC & 0x80;
}


void forward (void) //both wheels forward
{
	PORTA=0x06;
}

void left(void)	//left motor forward and right motor backward
{
	
	PORTA=0x05;
}


void right(void)	//right motor forward and left motor backward
{
	
	PORTA=0x0A;
}



void back (void) //both wheels backward
{
	PORTA=0x09;
}

void stop (void)	//both motors stop
{
	PORTA=0x00;
}


SIGNAL(SIG_USART0_RECV) 		// ISR for receive complete interrupt
{
	cli();
	arr[j++] = UDR0; 				//making copy of data from UDR1 in 'data' variable

	if(j==16)			// condition to check if 16 bytes received
	{
		

		j=0;			// start from the first byte
		lcd_print(1,1,(int)arr[0],3); //start byte is 0x7E ie., 126 in decimal
		lcd_print(1,5,(int)arr[11],3);		// Display the upper byte of the Yout of accelerometer
		lcd_print(1,9,(int)arr[12],3);		// Display the lower byte of the Yout of accelerometer
		lcd_print(2,1,(int)arr[13],3);		// Display the upper byte of the Xout of accelerometer
		lcd_print(2,5,(int)arr[14],3);		// Display the lower byte of the Xout of accelerometer 
		
	

yh=(int)(arr[11]);		//Store the upper byte of Yout of accelerometer in variable yh
yl=(int)(arr[12]);		//Store the lower byte of Yout of accelerometer in variable yl
xh=(int)(arr[13]);		//Store the upper byte of Xout of accelerometer in variable xh
xl=(int)(arr[14]);		//Store the lower byte of Xout of accelerometer in variable xl

//Setting flag values for different orientations of the accelerometer by declaring threshold values 

if((xh==001 && xl>220 && yh==002 && yl<040))		 h=1;		//Stop
if((xh==001 && xl<170 && xl>50 && yh==002 && yl>05))	 f1=1;		//Slow forward
if((xh==001 && xl<50 && xl>01 && yh==002 && yl>05))	 f2=1;		//Fast forward
if((xh==002 && xl>30 && xl<100 && yh==002 && yl>05))	 b1=1;		//Slow back
if((xh==002 && xl>100 && xl<255 && yh==002 && yl>05))	 b2=1;		//Fast back
	
if((xh==001 && xl>220 && yh==002 && yl>50 && yl<150))	 r1=1;		//Slow Right
if((xh==001 && xl>220 && yh==002 && yl>150 && yl<255))	 r2=1;		//Fast Right
if((xh==001 && xl>220 && yh==001 && yl<230 && yl>100))	 l1=1;		//Slow Left
if((xh==001 && xl>220 && yh==001 && yl<100 && yl>0))	 l2=1;	 	//Fast Right
		
		
		if(l1==1)
		{
			lcd_cursor(2,10);
			lcd_string("left1  ");
			velocity (150, 150);				//Set the velocity for slow motor movement
			left();						//Turn Left	
			_delay_ms(50);
		}

		if(r1==1)
		{
			lcd_cursor(2,10);
			lcd_string("right1");
			velocity (150, 150);				//Set the velocity for slow motor movement
			right();					//Turn Right
			_delay_ms(50);
		}

		if(f1==1)
		{
			lcd_cursor(2,10);
			lcd_string("fwd1   ");
			velocity (150, 150);				//Set the velocity for slow motor movement
			forward();					//Turn Forward
		_delay_ms(50);
		}

		if(b1==1)
		{
			lcd_cursor(2,10);
			lcd_string("back1  ");
			velocity (150, 150);				//Set the velocity for slow motor movement
			back();						//Move Back
			_delay_ms(50);
		}

		if(l2==1)
		{
			lcd_cursor(2,10);
			lcd_string("left2  ");
			velocity (255, 255);				//Set the velocity for fast motor movement
			left();						//Turn Left
			_delay_ms(100);
		}

		if(r2==1)
		{
			lcd_cursor(2,10);
			lcd_string("right2");
			velocity (255, 255);				//Set the velocity for fast motor movement
			right();					//Turn Right
			_delay_ms(100);
		}
		if(f2==1)
		{
			lcd_cursor(2,10);
			lcd_string("fwd2   ");
			velocity (255, 255);				//Set the velocity for fast motor movement
			forward();					//Move Forward
			_delay_ms(100);
		}		

		if(b2==1)
		{
			lcd_cursor(2,10);
			lcd_string("back2  ");
			velocity (255, 255);				//Set the velocity for fast motor movement
			back();						//Move Back
		_delay_ms(100);
		}
		
		
		if(h==1)
		{
			lcd_cursor(2,10);
			lcd_string("steady   ");
			stop();
					}

h=f1=f2=b1=b2=r1=r2=l1=l2=0;

		
	}
sei();
}



//Function To Initialize all The Devices
void init_devices()
{
 	cli(); //Clears the global interrupts
 	timer5_init();

	 uart0_init(); //Initailize UART1 for serial communiaction
	lcd_port_config();  //Initializes all the ports
	motion_pin_config();
 	sei();   //Enables the global interrupts
}


//Main Function
int main(void)
{
	h=f1=f2=b1=b2=r1=r2=l1=l2=0;
	init_devices();
	
	lcd_init();
	lcd_string("hi");
	_delay_ms(100);
	while(1);
	{
		
	}		
	
}

