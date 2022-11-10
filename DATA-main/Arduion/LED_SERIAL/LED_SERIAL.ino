#include <ESP8266WiFi.h>
 
int ledPin =16; 
 
void setup() {
  Serial.begin(115200);
  delay(10);
 
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);
  digitalWrite(ledPin, HIGH); 
  
}
 
void loop()
{
  digitalWrite(ledPin, LOW);
   Serial.println("LED OFF");
    delay(2000);

    digitalWrite(ledPin, HIGH);
   Serial.println("LED ON");
    delay(2000);
    
}
