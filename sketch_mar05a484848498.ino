#include <SoftwareSerial.h>
SoftwareSerial esp8266(9,3); //Pin 9 & 3 of Arduino as RX and TX. Connect TX and RX of ESP8266 respectively.
#include <math.h>
#include <LiquidCrystal.h>

#define DEBUG true
#define power 11 //power is connected to Pin 11 of Arduino

LiquidCrystal lcd(1,2,4,5,6,7);
int pinOut = 10;
int ledpin=13;
int buttonpin=8;
double Thermistor(int RawADC) {
 double Temp;
 Temp = log(10000.0*((1024.0/RawADC-1))); 
 Temp = 1 / (0.001129148 + (0.000234125 + (0.0000000876741 * Temp * Temp ))* Temp );
 Temp = Temp - 273.15;          
 //Temp = (Temp * 9.0)/ 5.0 + 32.0; 
 return Temp;
}




void setup()
  {
    pinMode(power, OUTPUT);
    digitalWrite(power, LOW);
    Serial.begin(9600);
    esp8266.begin(115200); //Baud rate for communicating with ESP8266. Your's might be different.
    esp8266Serial("AT+RST\r\n", 5000, DEBUG); // Reset the ESP8266
    esp8266Serial("AT+CWMODE=1\r\n", 5000, DEBUG); //Set station mode Operation
    esp8266Serial("AT+CWJAP=\"SSID\",\"Password\"\r\n", 5000, DEBUG);//Enter your WiFi network's SSID and Password.
                                   
    while(!esp8266.find("OK")) 
    {
      }
    esp8266Serial("AT+CIFSR\r\n", 5000, DEBUG);//You will get the IP Address of the ESP8266 from this command. 
    esp8266Serial("AT+CIPMUX=1\r\n", 5000, DEBUG);
    esp8266Serial("AT+CIPSERVER=1,80\r\n", 5000, DEBUG);
  pinMode(10, OUTPUT);
 lcd.begin(16, 2);
 //Serial.begin(9600);
 pinMode(ledpin,OUTPUT);
 pinMode(buttonpin,INPUT);

  }

void loop()
  {
  int val;                
  double temp;            
  val=analogRead(0);      
  temp=Thermistor(val); 
  //Serial.print("Temperature=");
  //Serial.print(temp);
  //Serial.println(" c");
  

    if (esp8266.available())
      {
        if (esp8266.find("+IPD,"))
          {
            String msg;
            esp8266.find("?");
            msg = esp8266.readStringUntil(' ');
            String command1 = msg.substring(0, 3);
            String command2 = msg.substring(4);
                        
            if (DEBUG) 
              {
                Serial.println(command1);//Must print "led"
                Serial.println(command2);//Must print "ON" or "OFF"
              }
            delay(100);

              if (command2 == "ON") 
                    {
                      digitalWrite(power, HIGH);
                    }
                   else 
                     {
                       digitalWrite(power, LOW);
                     }
          }
      }

  lcd.setCursor(2,1); 
  lcd.print("Temp = ");
    lcd.print(temp);   
  lcd.print(" c");
  delay(5000);            
  lcd.clear();
  if (temp >= 30){
    digitalWrite(pinOut, HIGH);
    lcd.print("Fan on");
    delay(1000);
  }

  else {
    digitalWrite(pinOut, LOW);
    lcd.print(" Fanoff");
  
       delay(500); 
  }
          
     if(digitalRead(buttonpin)==1){
    digitalWrite(ledpin,HIGH);
      
        lcd.print(" LED on");
        delay(1000);
  } 
     
  else {
   
      digitalWrite(ledpin,LOW);
    lcd.print("LED off");
    delay(500); 
  }

  }
   
String esp8266Serial(String command, const int timeout, boolean debug)
  {
    String response = "";
    esp8266.print(command);
    long int time = millis();
    while ( (time + timeout) > millis())
      {
        while (esp8266.available())
          {
            char c = esp8266.read();
            response += c;
          }
      }
    if (debug)
      {
        Serial.print(response);
      }
    return response;
  }
