/*
In this experiment, we interface a PIR(Passive InfraRed) Sensor with the Firebird V Robot

Hardware Connections that should be done before burning the C Code in the Firebird V Robot:
	-------------------------------------------------------------------------
	|		PIR Pins		|		Microcontroller Expansion Slot Pin		|
	-------------------------------------------------------------------------
	|		Ground			|		Pin 21/22 or any Ground Pin				|
	-------------------------------------------------------------------------
	|		+V				|		Pin 23/24 or any +5V supply				|
	-------------------------------------------------------------------------
	|	Digital Output		|		Any GPIO Pin but for now PL6 pin		|
	-------------------------------------------------------------------------

*/

#define F_CPU 14745600

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "scan_pir.h"

//Main Function
int main(void)
{
scan_pir();				//Function to scan the PIR Sensor when the PIR Digital Output pin is connected to the PORT L Pin 7
						//if the output is HIGH, then the LCD will display "Human Detected" and the Bargraph LEDs will be ON
						//if the output is LOW, then the LCD will display "Empty Space" and the Bargraph LEDs will be OFF
}
