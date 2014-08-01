Functions used in the accelerometer.h header ?le
1. acc init devices() :
void acc_init_devices() 
{
cli(); //Clear Interrupt 
port_init(); //Initialise the ADC & LCD Port pins 
lcd_init(); //Initialise LCD 
adc_init(); //Initialise ADC 
sei(); //Clear Interrupt 
}

2. acc init devices() :
void acc_process(void) 
{ 
x= ADC_Conversion(14); //ADC Conversion of X out 
y= ADC_Conversion(15); //ADC Conversion of Y out 
z= ADC_Conversion(11); //ADC Conversion of Z out
//The code below can be uncommented to check if the //values obtained by the ADC for the Accerometer are // accurate or not
/* lcd_cursor(1,1); lcd_string("X = "); lcd_print(1,4,x,3); 
lcd_cursor(1,7); lcd_string(" y = "); lcd_print(1,11,y,3); 
lcd_cursor(2,1); lcd_string("z = "); lcd_print(2,4,z,3) ;*/
}

3. acc get x() :
unsigned char acc_get_x(void) { return(x); //return the value of x }

4. acc get y() :
unsigned char acc_get_y(void) { return(y); //return the value of y }

5. acc get z() :
unsigned char acc_get_z(void) { return(z); //return the value of z }