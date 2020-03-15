#include <Adafruit_GPS.h>
#include <TimeAlarms.h>


int relay1 = 7;  //This is the Arduino Pin that will control Relay #1
int relay2 = 6;  //This is the Arduino Pin that will control Relay #2
//int relay3 = 5;  //This is the Arduino Pin that will control Relay #3
//int relay4 = 8;  //This is the Arduino Pin that will control Relay #4


// Connect the GPS Power pin to 5V
// Connect the GPS Ground pin to ground
// Connect the GPS TX (transmit) pin to Digital 19
// Connect the GPS RX (receive) pin to Digital 18
Adafruit_GPS GPS(&Serial1);
// Set GPSECHO to 'false' to turn off echoing the GPS data to the Serial console
// Set to 'true' if you want to debug and listen to the raw GPS sentences
#define GPSECHO  false

char Eber1;
char Eber2;
String GPS_string1;        // format the value for Google maps style
String GPS_string2;        // format the value for Google maps style
String GPS_string3;        // format the value for Google maps style
String GPS_string;
int timer_e1,timer_e2;
String textMessage;        // Variable to store text message
String SMS_response;      // 
char timeout_rx_sms;


void setup() {
  // put your setup code here, to run once:
  pinMode(relay1, OUTPUT);
  pinMode(relay2, OUTPUT);
  //pinMode(relay3, OUTPUT);
  //pinMode(relay4, OUTPUT);
  digitalWrite(relay1, LOW);
  digitalWrite(relay2, LOW);
  //digitalWrite(relay3, LOW);
  //digitalWrite(relay4, LOW);
  pinMode (13, OUTPUT); // led 

  Serial1.begin(9600, SERIAL_8E1); // GPS serial port
  Serial2.begin(115200); // GSM serial port
  Serial.begin(115200);       // for debugging
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }

 // 9600 NMEA is the default baud rate for Adafruit MTK GPS's- some use 4800
  GPS.begin(9600);

  // uncomment this line to turn on RMC (recommended minimum) and GGA (fix data) including altitude
  //GPS.sendCommand(PMTK_SET_NMEA_OUTPUT_RMCGGA);
  // uncomment this line to turn on only the "minimum recommended" data
  GPS.sendCommand(PMTK_SET_NMEA_OUTPUT_RMCONLY);
  // For parsing data, we don't suggest using anything but either RMC only or RMC+GGA since
  // the parser doesn't care about other sentences at this time

  // Set the update rate
  GPS.sendCommand(PMTK_SET_NMEA_UPDATE_1HZ);   // 1 Hz update rate
  // For the parsing code to work nicely and have time to sort thru the data, and
  // print it out we don't suggest using anything higher than 1 Hz

  // Request updates on antenna status, comment out to keep quiet
  //  GPS.sendCommand(PGCMD_ANTENNA);

  delay(1000);
  Serial.println("Initializing..."); 
  Serial2.println("AT"); //Once the handshake test is successful, it will back to OK
  updateSerial();
  Serial2.println("AT+CSQ"); //Signal quality test, value range is 0-31 , 31 is the best
  updateSerial();
  Serial2.println("AT+CCID"); //Read SIM information to confirm whether the SIM is plugged
  updateSerial();  
  //Check whether it has registered in the network
    while( (sendATcommand("AT+CREG?", "+CREG: 0,1", 500) ||
            sendATcommand("AT+CREG?", "+CREG: 0,5", 500)) == 0 );
  Serial2.println("AT+CMGF=1"); // Configuring SMS TEXT mode
  updateSerial();
  Serial2.println("AT+CNMI=1,2,0,0,0"); // Decides how newly arrived SMS messages should be handled
  updateSerial();  
  Serial2.println("AT+CMGDA=DEL ALL");   // delete all SMS
  updateSerial();    
}

uint32_t timer = millis();
// **********************************************************************************  


void loop() {
  // put your main code here, to run repeatedly:
  char c = GPS.read();
  // if you want to debug, this is a good time to do it!
  if ((c) && (GPSECHO))
    Serial.write(c);   

  // if a sentence is received, we can check the checksum, parse it...
  if (GPS.newNMEAreceived()) {
    // a tricky thing here is if we print the NMEA sentence, or data
    // we end up not listening and catching other sentences!
    // so be very wary if using OUTPUT_ALLDATA and trytng to print out data
    //Serial.println(GPS.lastNMEA());   // this also sets the newNMEAreceived() flag to false

    if (!GPS.parse(GPS.lastNMEA()))   // this also sets the newNMEAreceived() flag to false
      return;  // we can fail to parse a sentence in which case we should just wait for another
  }
  // print GPS di debug 
 /*
      Serial.print("\nTime: ");
      if (GPS.hour < 10) { Serial.print('0'); }
      Serial.print(GPS.hour, DEC); Serial.print(':');
      if (GPS.minute < 10) { Serial.print('0'); }
      Serial.print(GPS.minute, DEC); Serial.print(':');
      if (GPS.seconds < 10) { Serial.print('0'); }
      Serial.println(GPS.seconds, DEC);
      Serial.print("Date: ");
      Serial.print(GPS.day, DEC); Serial.print('/');
      Serial.print(GPS.month, DEC); Serial.print("/20");
      Serial.println(GPS.year, DEC);
      Serial.print("Fix: "); Serial.print((int)GPS.fix);
      Serial.print(" quality: "); Serial.println((int)GPS.fixquality);
      if (GPS.fix) {
        Serial.println("Location: ");
        GPS_string1 = String(GPS.latitude,4);  // format data in google maps style DMM format
        GPS_string2 = GPS_string1.substring(0,GPS_string1.length()-7) + " " + GPS_string1.substring(GPS_string1.length()-7)+" ";
        Serial.print(GPS_string2);
        GPS_string1 = String(GPS.longitude,4);
        GPS_string2 = GPS_string1.substring(0,GPS_string1.length()-7) + " " + GPS_string1.substring(GPS_string1.length()-7);
        Serial.println(GPS_string2);
        Serial.print("Speed (knots): "); Serial.println(GPS.speed);
        Serial.print("Angle: "); Serial.println(GPS.angle);
        Serial.print("Altitude: "); Serial.println(GPS.altitude);
        Serial.print("Satellites: "); Serial.println((int)GPS.satellites);
      }
    */
// **********************************************************************************  
  // if millis() or timer wraps around, we'll just reset it
  if (timer > millis())  timer = millis();
  // approximately every 1 seconds or so, print out the current stats
  if (millis() - timer > 1000) {
    timer = millis(); // reset the 1" timer

    if (timer_e1)    // timeout riscaldamento 1
      {
      timer_e1--;
//      Serial.println(timer_e1);
      }
    else    
      Eber1 = 0;
    if (timer_e2)    // timeout riscaldamento 2
      {
      timer_e2--;
 //     Serial.println(timer_e2);      
      }
    else    
      Eber2 = 0;
    
    if (Eber1)    // gestisci relè 1     
      digitalWrite(relay1, HIGH);
    else
      digitalWrite(relay1, LOW);

    if (Eber2)// gestisci relè 2
      digitalWrite(relay2, HIGH);
    else
      digitalWrite(relay2, LOW);

  if (digitalRead(13))        // led blink
    digitalWrite(13, LOW);
  else
      digitalWrite(13,HIGH);

// **********************************************************************************  
// confeziona il messaggio GPS
  GPS_string = "Ora: ";
  if (GPS.hour < 10) {
      GPS_string +='0';
  }
  GPS_string += String(GPS.hour, DEC); GPS_string +=':';
  if (GPS.minute < 10) {
    GPS_string +='0';
  }
  GPS_string += String(GPS.minute, DEC); GPS_string += ':';
  if (GPS.seconds < 10) {
    GPS_string += "0\r";
  }
  GPS_string += String(GPS.seconds, DEC);
  GPS_string += " Data: ";
  GPS_string += String(GPS.day, DEC); GPS_string +='/';
  GPS_string += String(GPS.month, DEC); GPS_string +="/20";
  GPS_string += String(GPS.year, DEC);
  GPS_string += "\n";
  //    Serial.print("Fix: "); Serial.print((int)GPS.fix);
  //    Serial.print(" quality: "); Serial.println((int)GPS.fixquality);
  if (GPS.fix) {
//    GPS_string +="<a href=\"";    
    GPS_string1 = String(GPS.latitude, 4); // format data in google maps style DMM format
    GPS_string2 = GPS_string1.substring(0, GPS_string1.length() - 7) + " " + GPS_string1.substring(GPS_string1.length() - 7) + " ";
    GPS_string1 = String(GPS.longitude, 4);
    GPS_string3 = GPS_string1.substring(0, GPS_string1.length() - 7) + " " + GPS_string1.substring(GPS_string1.length() - 7);
//    GPS_string +="https://maps.google.com/maps?q=";
    GPS_string +="Pos :";
    GPS_string += GPS_string2;
    GPS_string += GPS_string3;
    GPS_string += "\n";        
//    GPS_string += GPS_string2;
//    GPS_string += "N ";        
//    GPS_string += GPS_string3;    
//    GPS_string += "E</a>";         
//    GPS_string += "\n";
    GPS_string += "Speed (kn): "; GPS_string += String(GPS.speed);
    GPS_string += (" A.: "); GPS_string += String(GPS.angle);
    //      Serial.print("Altitude: "); Serial.println(GPS.altitude);
    GPS_string += " Sat.: "; GPS_string += String((int)GPS.satellites);
//    Serial.println(GPS_string); 
    }   
}
// **********************************************************************************  
// If there are any SMSs available()

  if(Serial2.available()>0)
    textMessage = Serial2.readString();  
  if (textMessage.indexOf ("\0")>=0)    
  {
//   Serial.println(textMessage);      
    textMessage.toUpperCase();      // all upper case
//**************************************************************************************************************    
//**************************************************************************************************************    
    if(textMessage.indexOf("+CMT: \"+ZZXXXXXXXXXXXX\"")>=0){  // change here your number with prefix
      if (textMessage.indexOf("E1 ON")>=0)   
        {
        SMS_response += "Eberspacher 1 acceso \n";
        Eber1 = true;
        timer_e1 = 28800;
        }
      if (textMessage.indexOf("E1 OFF")>=0)   
        {
        SMS_response += "Eberspacher 1 spento \n";
        timer_e1 = 0;      
        Eber1 = false;
        }
      if (textMessage.indexOf("E2 ON")>=0)   
        {
        SMS_response += "Eberspacher 2 acceso \n";      
        Eber2 = true;
        timer_e2 = 28800;      
        }
      if (textMessage.indexOf("E2 OFF")>=0)   
        {
        SMS_response += "Eberspacher 2 spento \n";   
        timer_e2 = 0; 
        Eber2 = false;
        } 
      Serial2.println("AT+CMGS=\"+393355328187\"");  
      delay(100);
      Serial2.println(SMS_response); 
      Serial2.println(GPS_string);            
      delay(500);
      Serial2.write(26);       
//      Serial.print(SMS_response);    
      SMS_response = "";
      GPS_string ="";
      delay(100);   
      textMessage ="";
    }
   Serial.print(textMessage);   
   delay(100);   
   textMessage ="";
  }
}

//*********************************************************************************************************
void updateSerial()
{
  delay(500);
  while (Serial.available()) 
  {
    Serial2.write(Serial.read());//Forward what Serial received to Software Serial Port
  }
  while(Serial2.available()) 
  {
    Serial.write(Serial2.read());//Forward what Software Serial received to Serial Port
  }
}

int8_t sendATcommand(char* ATcommand, char* expected_answer, unsigned int timeout){

    uint8_t x=0,  answer=0;
    char response[100];
    unsigned long previous;

    memset(response, '\0', 100);    // Initialice the string
   
    delay(100);
   
    while( Serial2.available() > 0) Serial2.read();    // Clean the input buffer
   
    Serial2.println(ATcommand);    // Send the AT command


    x = 0;
    previous = millis();

    // this loop waits for the answer
    do{
        // if there are data in the UART input buffer, reads it and checks for the asnwer
        if(Serial2.available() != 0){   
            response[x] = Serial2.read();
            x++;
            // check if the desired answer is in the response of the module
            if (strstr(response, expected_answer) != NULL)   
            {
                answer = 1;
                Serial.print(response);
            }
        }
    // Waits for the asnwer with time out
    }while((answer == 0) && ((millis() - previous) < timeout));   

    return answer;
}

    
