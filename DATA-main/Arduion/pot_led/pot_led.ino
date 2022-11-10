#include <ESP8266WiFi.h>
//int ledpin = 16;
void setup()
{
Serial.begin(115200);
delay(2000);

pinMode(A0,INPUT); 

 pinMode(16, OUTPUT);
  digitalWrite(16, LOW);
 
}
void loop()
{
//int ledpin = digitalWrite(16);
int potencia = analogRead(A0);
Serial.println(potencia);
delay(2000);

//potencia = 1023 - potencia;
if(potencia>=800)
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
}
