//#include<ESP8266WiFi.h>
#include "UbidotsMicroESP8266.h"

#define ID "5ce99486c03f9741ea75fb78"  // Put here your Ubidots variable ID

#define TOKEN "A1E-WUG6CI5HcGpFinHs6JDlWkHg7VCDPKmXygLv1oxLNdRWwkEDP75TZy0H"  // Put here your Ubidots TOKEN
#define WIFISSID "Redmi" // Put here your Wi-Fi SSID
#define PASSWORD "mayur123" // Put here your Wi-Fi password

Ubidots client(TOKEN);

void setup()
{

pinMode(A0,INPUT); 

 pinMode(16, OUTPUT);
  digitalWrite(16, LOW);
  Serial.begin(9600);
//delay(2000);
  client.wifiConnection(WIFISSID, PASSWORD);
 
}
void loop()
{
  

int potv = analogRead(A0);
Serial.println(potv);//display on serial monitor

int value2 = client.getValue(ID);
  analogWrite(A0,value2);
  
if(potv>=800)
{
   digitalWrite(16, HIGH);
   Serial.println("LED ON");
    delay(2000);
}
   else
  {
     digitalWrite(16, LOW);
   Serial.println("LED OFF");
    delay(2000);
  }
    client.add("value2",potv);
    client.sendAll(true);
    delay(1000);
}
