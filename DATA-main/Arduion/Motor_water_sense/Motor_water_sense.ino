#define SensorPin 4  // used for ESP32
#define motorpin1 26
#define motorpin2 27

void setup() { 
  Serial.begin(115200);
  pinMode(motorpin1, OUTPUT);
  pinMode(motorpin2, OUTPUT);
}

void loop() {
  float sensorValue = analogRead(SensorPin);
  Serial.println(sensorValue);
//  delay(3000);

  if(sensorValue<1500)
  {
    motorup();
    delay(5000);
    motorstop();
    delay(1000);
    motordown();
    delay(5000);
  }
} 

void motorup()
{
  digitalWrite(motorpin1, HIGH);
  digitalWrite(motorpin2, LOW);
}

void motordown()
{
  digitalWrite(motorpin1, LOW);
  digitalWrite(motorpin2, HIGH);
}
void motorstop()
{
  digitalWrite(motorpin1, LOW);
  digitalWrite(motorpin2, LOW);
}
