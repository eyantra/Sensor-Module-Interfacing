This project encapsulates the work done by the team for PIR Sensor at the eYantra Summer Internship 2014 from 26th May to 10th July 2014.
---------------------------
Team Members
Shantanu Sengupta
Chayatan
Mukilan
---------------------------
Mentor
Parin Cheddha
---------------------------
Code Explanation

PIR Sensor is used to detect Human Motion. When a human passes in front of this PIR Sensor, it creates a pulse.

The code interfaces the PIR Sensor module with the FireBird V Robot. 

The PIR Sensor is connected to the PL6 Pin of the Microcontroller, and its corresponding output can be observed on the Bargraph LEDs and in the LCD.

The code consists of two parts. Firstly, the initialisation part, where all the ports for PIR, Bargraph LED and LCD are initialised.
void PIR();
void LED();
void lcd_port_config();

All these 3 Ports are initialised using port_init() function. 
Including Interrupts to the above function, init_devices() function is created.


And secondly, the main program, where the value of the PIR Sensor output is detected and corresponding output is generated in the Batgraph LED and LCD. 
The main program is called using the function : scan_pir().

---------------------------