What does this code do?
==================
The basic concept behind interfacing ultrasonic sensor is to receive the analog value from the sensor and store it in a register. 
The sensor sends 10 bit data. So the ADLAR register helps us to store it in the desired format in the ADCL and ADCH registers. 
Once the resultant value is got the value should be calibrated in such a way that it gives the exact values in centimeter. 
This is what this code does exactly.
The various functions involved in the code and their functions are listed below.

"'ADC_conversion()'" - receives the 10 bit analog value, converts it into a single 
8 bit value and stores it in a variable 'a'.

"ultra()" - will print the value in our desired format. The scaling factor included 
in this function will 
determine the unit in which we represent the data.



What does the header file do?
======================
Including ultrasonic.h in any application  program gives us the benefit of using 
any function which comes under ultrasonic.h. This is useful in application programs.