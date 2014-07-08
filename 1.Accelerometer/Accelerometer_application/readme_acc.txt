Accelerometer application program built on fireBired V




in this program, we used the following

1. timers: timers are used to set the pwm for the robot to move at different
	   speeds.

2. ADC:    ADC are used to convert the analog value of the XY and z coordinates
 	   into their digital equivalent.

3. Zigbee module: this module is used to transfer data wirelessly from the	                  sensor to the Robot.

3. UART0:  UART serial communication module is used to receive the data sent by
   	   the zigbee module serially.

4. LCD:    The lcd screen is used to display the digital values of the co-	   ordinates and the robotic movement indications like, left, right,back, front
   etc..

steps involved:

1. power up the accelerometer
2. configure the two Zigbee modules
3. connect the accelerometer to one of the zigbee module and the other to the
   FireBird V robot.
4. Load the program to the FireBird V
5. enjoy the application by changing the sensor orientation



Zigbee configuration

1. connect the two zigbee module to the PC
2. test for their working
3. assign the source and destination addresses to each of the zigbee modules
4. assign the 3 ADC input pins of the transmitting Zigbee module 
   (pin 11-->x and 17--y) z can be ommited (refer zigbee data sheet)
5. provide the vref =3.3V to the transmitting Zigbee
6. place the receiving Zigbee module on the FireBird V robot
7. provide power supply and ground to all the module properly

Now Enjoy the joystick look and feel....

					By, Chayatan



