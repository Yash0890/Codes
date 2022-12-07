#define Liquid_Detection_Pin 2
  
#include<Servo.h>
Servo Myservo;
int pos;
void setup() {
  Serial.begin(9600);
  pinMode(Liquid_Detection_Pin, INPUT);
  Myservo.attach(3);
}

void loop() {
  
  if (digitalRead(Liquid_Detection_Pin)) 
  {
    Serial.println("Liquid Detected!");
  }
  else 
  {
    Serial.println("No Liquid!");
    for(pos=0;pos<=90;pos++)
    {
       Myservo.write(pos);
       delay(15);
    }
    delay(1000);
    for(pos=90;pos<=0;pos++)
    {
       Myservo.write(pos);
       delay(15);
    }
    delay(1000);
  }

  delay(1000);
}