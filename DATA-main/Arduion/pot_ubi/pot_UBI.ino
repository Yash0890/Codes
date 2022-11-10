//#include<ESP8266WiFi.h>
#include "UbidotsMicroESP8266.h"

#define ID "5ce99f5bc03f974c601f8654"  // Put here your Ubidots variable ID

#define TOKEN "A1E-WUG6CI5HcGpFinHs6JDlWkHg7VCDPKmXygLv1oxLNdRWwkEDP75TZy0H"  // Put here your Ubidots TOKEN
#define WIFISSID "Redmi" // Put here your Wi-Fi SSID
#define PASSWORD "mayur123" // Put here your Wi-Fi password

Ubidots client(TOKEN);

void setup()
{

  pinMode(A0,INPUT); 
  Serial.begin(9600);
//delay(2000);
  client.wifiConnection(WIFISSID, PASSWORD);
 
}
void loop()
{
int potv = analogRead(A0);
Serial.println(potv);//display on serial monitor

int value4 = client.getValue(ID);
  analogWrite(A0,value4);
  
    client.add("value4",potv);
    client.sendAll(true);
    delay(1000);
}
