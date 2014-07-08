The sensor, senses orientation of the particular axis and accordingly gives the analog output on the Xout, Yout and Zout pins.
Therefore we have defined a header file called as accelerometer.h where we call the following functions that are used in interfacing the Accelerometer to the Firebird V Robot.
This header file can be found in the Headers folder.
The following functions are used in this header file:
acc_init_devices() function : This function initializes all the
 ADC ports and configures the respective ADC registers.

acc_process() : This function is used to compare the ADC values
 with the threshold, make the oriental decision and updates the
 left, right, forward and backward flags.

acc_get_x() : This function will get the ADC values of the 
X co-ordinate of the Accelerometer when connected to the ADC

acc_get_y() : This function will get the ADC values of the 
Y co-ordinate of the Accelerometer when connected to the ADC

acc_get_z() : This function will get the ADC values of the 
Z co-ordinate of the Accelerometer when connected to the ADC
