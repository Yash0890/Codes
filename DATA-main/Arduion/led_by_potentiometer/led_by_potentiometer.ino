#include<Servo.h>
Servo myservo;
void setup()
{
  pinMode(4,INPUT);
  myservo.attach(3);
  digitalWrite(4,HIGH);
}

void loop()
{
  if(digitalRead(4) == LOW)
  {
    myservo.write(170);
  }
  else
 {
    myservo.write(0);
  }
}
/*
 *    #dht scensor values on mobile 
 */
