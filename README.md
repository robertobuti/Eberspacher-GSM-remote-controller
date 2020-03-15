# Eberspacher-GSM-remote-controller
Remote controller for Eberspacher heater
This project born from need to swtich on the Eberspacher heaters on my boat some hours before use it, in order to find warn environment, nothing else.
Obviosuly exist similar device from manufacturer, but the price is relevant, so see here a easy solution.
I have done the first version from a webserver and usign 4G wifi connection, but too much problem with phone operators, noone offer pubblic IP so a VPN is required, too complex. I switch to simple 2G GSM connection using SMS, with IOT sim car is very cheap solution.
HW requirement :
- Arduino CPU, i have used ATmega 2560
- GSM module, i have used SIM800L
- Switching power supply for GSM module
- GPS Module
- Relay shield board
The board will be alway on, like power supply on Eberspacher, on my boat i have two heater, so the project is design to drive two relay output, more then i add a GPS module so the systemc an be used as tracker, few euro material and nice function.
Some considerations about GSM :
power supply require high current when radio is switched on, GSM voltage range is from 3.4 to 4.2, so an external switching regulator is raccomanded, trim it abotu at 3.6V, it's fine like Lithium battery voltage.
Again on GSM, Arduino in compatibile with 3.3V signal input, but signal are 5V TTL, so TX from serial port can be connected directly to RX of GSM, voltage need to be reduced at comaptible level, nothing of difficult, just use two resistor (10K e 22K) is all you need. Search on google for more information, here a good link. :https://lastminuteengineers.com/sim800l-gsm-module-arduino-tutorial/ 

 ![Eberspacher-GSM-remote-controller](/gsm_resistors.png )
