
#define F_CPU 14745600
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "lcd.h"

#define	SLA_W	0xD2             // Write address for DS1307 selection for writing
#define	SLA_R	0xD3
#define MAX_BUFFER_SIZE 	16

signed int X_Data,Y_Data,Z_Data=0;

float Displacement;
signed int X_Disp,Y_Disp,Z_Disp=0;


void lcd_port_config(void)		//configure all the lcd i/0 ports
{
	DDRC = DDRC | 0xF7;
	PORTC = PORTC & 0x80;
}


void uart3_init(void)			// configure serial communication
{
	UCSR3B = 0x00; //disable while setting baud rate
	UCSR3A = 0x20;
	UCSR3C = 0x06;//(1<<USBS3) | (3<<UCSZ30);
	UBRR3L = 0x5F;//set baud rate to 9600bps
	UBRR3H = 0x00;
	UCSR3B = 0x98;//(1<<RXEN3) | (1<<TXEN3);
	
}


unsigned char chartobcd(unsigned char n)	// This function is used to convert the characters to Bcd
{
	return ((n / 10) << 4) | (n % 10);
}

void twi_init(void)
{
	TWCR = 0x00; //disable twi
	TWBR = 0x10; //set bit rate
	TWSR = 0x00; //set prescale
	TWAR = 0x00; //set slave address
	TWCR = 0x04; //enable twi
}
void write_byte(unsigned char data_out,unsigned char address)
{
	TWCR = (1<<TWINT) | (1<<TWSTA) | (1<<TWEN);       // send START condition
	while(!(TWCR & (1<<TWINT)));                      // wait for TWINT Flag set
	_delay_ms(10);
	

	TWDR = SLA_W;                                     // load SLA_W into TWDR Register
	TWCR  = (1<<TWINT) | (0<<TWSTA) | (1<<TWEN);      // clear TWINT flag to start tramnsmission of slave address
	while(!(TWCR & (1<<TWINT)));                      // wait for TWINT Flag set
	_delay_ms(10);


	TWDR = address;                                   // send address of register byte want to access register
	TWCR  = (1<<TWINT) | (1<<TWEN);                   // clear TWINT flag to start tramnsmission of address

	while(!(TWCR & (1<<TWINT)));                      // wait for TWINT Flag set
	_delay_ms(10);
	

	TWDR = data_out;chartobcd(data_out);              // convert the character to equivalent BCD value and load into TWDR
	TWCR  = (1<<TWINT) | (1<<TWEN);                   // clear TWINT flag to start transmission of data byte
	while(!(TWCR & (1<<TWINT)));                      // wait for TWINT Flag set
	_delay_ms(10);

	TWCR = (1<<TWINT) | (1<<TWSTO) | (1<<TWEN);       // send STOP condition
}


unsigned char read_byte(unsigned char address)
{
	unsigned char acc_recv_data;

	TWCR = (1<<TWINT) | (1<<TWSTA) | (1<<TWEN);       // send START condition
	while(!(TWCR & (1<<TWINT)));                      // wait for TWINT Flag set
	_delay_ms(100);


	TWDR = SLA_W;									   // load SLA_W into TWDR Register
	TWCR  = (1<<TWINT) | (1<<TWEN);                   // clear TWINT flag to start transmission of slave address
	while(!(TWCR & (1<<TWINT)));                      // wait for TWINT Flag set
	_delay_ms(10);

	TWDR = address;                                   // send address of register byte want to access register
	TWCR  = (1<<TWINT) | (1<<TWEN);                   // clear TWINT flag to start transmission of slave address
	while(!(TWCR & (1<<TWINT)));                      // wait for TWINT Flag set
	_delay_ms(10);

	TWCR = (1<<TWINT) | (1<<TWSTA) | (1<<TWEN);       // send RESTART condition
	while(!(TWCR & (1<<TWINT)));                      // wait for TWINT Flag set
	_delay_ms(10);

	TWDR = SLA_R;									   // load SLA_R into TWDR Register
	TWCR  = (1<<TWINT) | (0<<TWSTA) | (1<<TWEN);      // clear TWINT flag to start transmission of slave address
	while(!(TWCR & (1<<TWINT)));                      // wait for TWINT Flag set
	_delay_ms(10);

	TWCR  = (1<<TWINT) | (1<<TWEN);                   // clear TWINT flag to read the addressed register
	while(!(TWCR & (1<<TWINT)));                      // wait for TWINT Flag set

	acc_recv_data = TWDR;
	_delay_ms(10);

	TWDR = 00;                                        // lOad the NO-ACK value to TWDR register
	TWCR  = (1<<TWINT) | (1<<TWEN);                   // clear TWINT flag to start transmission of NO_ACK signal
	while(!(TWCR & (1<<TWINT)));                      // wait for TWINT Flag set
	_delay_ms(10);
	
	return(acc_recv_data);                            // return the read value to called function
}

void init_devices()
{
	cli();              // disable all interrupts
	lcd_port_config();  // configure the LCD port

	twi_init();         // configurE the I2cC, i.e TWI module
	uart3_init();
	sei();              // re-enable interrupts
	//all peripherals are now initialized
}

int main(void)
{
	char xl[5],xh[5],yl=0,yh=0,zl=0,zh=0,x1,x2,y1,y2,z1,z2;
	int sign, x_dec,xdisp,xpre;
	int raw_x[5],raw_y=0,raw_z=0,n=0;
	float xrate[5],yrate,zrate;
	cli();
	sei();
	lcd_port_config();
	lcd_init();
	
	init_devices();
	
	lcd_init();                     // initialize the LCD with its commands
	
	write_byte(0x5F,0x2E);
	write_byte(0x10,0x23);
	write_byte(0x8F,0x20);			// normal mode and enable all x y z axis
	//	write_byte(0x40,0x24);
	
	
	l1: while(1)
	{
		_delay_ms(500);
		lcd_wr_command(0x01);			//clear lcd screen
		
		X_Data = read_byte(0x28);		//receive 8bit value from 'x low' register
		
		UDR3 = read_byte(0x29);			//transmit 8bit value of 'x high' register to usart3
		X_Data|= (signed short int)read_byte(0x29) << 8;	//convert the two 8bit values to one 16 bit data
		
		
		
		Y_Data = read_byte(0x2a);		//receive 8bit value from 'y low' register
		//UDR3=Y_Data;
		Y_Data|= (signed short int)read_byte(0x2b)<< 8;	//convert the two 8bit values 'y high and ylow' to one 16 bit data
		
		
		
		Z_Data = read_byte(0x2c);		//receive 8bit value from 'z low' register
		//UDR3=Z_Data;
		Z_Data|= (signed short int)read_byte(0x2d) << 8;//convert the two 8bit values 'z high and zlow' to one 16 bit data
		
		
		// this is a small try to calibrate the gyroscope coordinate values which turns out to be fuzzy.
		// 57.0,0.01, and 10 are all constants obtained by trial and error method
		
		
		Displacement = ((float)X_Data / 57.0) * 0.01 * 10.0;
		X_Data = (signed short int)Displacement;
		X_Disp=X_Data;
		lcd_cursor(1,1);
		lcd_string("X");
		lcd_cursor(2,1);
		lcdprintf("%d",X_Disp);
		
		Displacement = ((float)Y_Data / 57.0) * 0.01 * 10.0;
		Y_Data = (signed short int)Displacement;
		Y_Disp=Y_Data;
		lcd_cursor(1,6);
		lcd_string("Y");
		lcd_cursor(2,6);
		lcdprintf("%d",Y_Disp);

		Displacement = ((float)Z_Data / 57.0) * 0.01 * 10.0;
		Z_Data = (signed short int)Displacement;
		Z_Disp=Z_Data;
		lcd_cursor(1,11);
		lcd_string("Z");
		lcd_cursor(2,11);
		lcdprintf("%d",Z_Disp);

	}




}
