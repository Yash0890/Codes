#include<Servo.h>
#define Switch_pin 4
int switch_status =0, arm_rotation = 0;
Servo armservo;

void setup()
{
  armservo.attach(3);
  //pinMode(Switch_pin,INPUT);
}

void loop()
{
  switch_status = digitalRead(Switch_pin);
  if(switch_status == HIGH)
  {
    arm_rotation = random(1,4);
    if(arm_rotation == 1)
    {
      armservo.write(135);
      delay(500);
      armservo.write(80);
      delay(200);
    }
    if(arm_rotation == 2)
    {
      armservo.write(140);
      delay(500);
      armservo.write(80);
      delay(250);
      armservo.write(140);
      delay(550);
      armservo.write(5);
      delay(1500);
      armservo.write(120);
      delay(1000);
    }
    if(arm_rotation == 3)
    {
      armservo.write(120);
      delay(2000);
      armservo.write(30);
      delay(700);
      armservo.write(140);
      delay(850);
      armservo.write(80);
      delay(250);
      armservo.write(140);
      delay(550);
      armservo.write(5);
      delay(1500);
      armservo.write(120);
      delay(1000);
      armservo.write(5);
      delay(1200);
      armservo.write(120);
      delay(1500);
    }
  }
  else if(switch_status == LOW)
  {
    armservo.write(1);
  }
}
