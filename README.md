# Eberspacher-GSM-remote-controller
Remote controller for Eberspacher heater
This project born from need to swtich on the Eberspacher heaters on my boat some hours before use it, in order to find warn environment, nothing else.
Obviosuly exist similar device from manufacturer, but the price is relevant, so see here a easy solution.
I have done the first version from a webserver and usign 4G wifi connection, but too much problem with phone operators, noone offer pubblic IP so a VPN is required, too complex. I switch to simple 2G GSM connection using SMS, with IOT sim card is very cheap solution. (rember to disable the PIN, use your phone to do it)
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
Or see the picture on how i have done.

 ![Eberspacher-GSM-remote-controller](/gsm_resistors.png )

Genrally the hw is very simple, except the GSM that require a bit of attention, but finally is only power supply + RX/TX signals, in my case connected to serial port 2.
GPS module, connected directly to 5V for power supply and to serial port 1 for comunication, if need help use google , you will find a lot of info.
For the outputs i have used a relay shield V3.0, it's a board with 4 realys outputs, but i need only two, my be in future others will be useful, if use different relay board be sure to adjust pin number outputs on code. 
On Eberspacher is easy, from original controller you have 4 colore wire, red, yellow, grey/red, brown/white, use a relay contact to close RED (+12V) with YELLOW (swithc signal), very easy. For your convenience see at this link more information. https://www.letonkinoisvarnish.co.uk/eberspacher_intro_1.html

The full device assembled (without realy shield) is in following picture.
 ![Eberspacher-GSM-remote-controller](/controller.jpg )

I you use same HW you need only to adjust a code line, at line 231 sound like:
    if(textMessage.indexOf("+CMT: \"+ZZXXXXXXXXXXXX\"")>=0){  // change here your number with prefix
    
Change +ZZ with your local international prefix, XXXXXXXXXX with your mobile number, used to send message to the controller, only your phone will be recognized as authorized sender.

Message must be in followign format :

Letter E to identify the Eberspacher command, followed by number 1 or 2 for the heater, a space, and the status ON or OFF. 

Example of text to send:

E1 ON E2 ON , other variants at your choise, you can send also just E1 ON for example. Is not case sensitive, use also lower case if you prefere.
In response you will have a sms with Eberspacher status and GPS informations.

Note: if you send a SMS with any different message you will have back only GPS informations.

SMS received from unauthorized sender are trashed.

Enjoy
