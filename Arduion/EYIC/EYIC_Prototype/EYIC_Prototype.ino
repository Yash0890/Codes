#include "DHT.h"

#define Aout_pin 34

#define DHTPIN 13
#define DHTTYPE DHT22

#define relay_pin 33

int value_status = 0;

DHT dht(DHTPIN,DHTTYPE);

void setup(){

  pinMode(relay_pin,OUTPUT);

  pinMode(LED_BUILTIN,OUTPUT);
  Serial.begin(115200);

  dht.begin();
  
  digitalWrite(relay_pin, LOW);
  digitalWrite(LED_BUILTIN, LOW);
}

void loop(){
  delay(1500);
  
  // Plant 1
  int value1 = analogRead(Aout_pin);

   // Read Humidity 
  float Hum = dht.readHumidity();
  
  // Read temperature as Celsius (the default)
  float temp_c = dht.readTemperature();

  // Check if any reads failed and exit early (to try again).
  if (isnan(Hum) || isnan(temp_c) ) {
    Serial.println(F("Failed to read from DHT sensor!"));
    return;
  }

  Serial.print("Moisture Value :");
  Serial.println(value1);

  Serial.print("\nHumidity : ");
  Serial.println(Hum);

  Serial.print("\nTemperature : ");
  Serial.println(temp_c);

  Serial.print("\nValve Status :");

  if(value1 <= 2300)
    value_status = 1;
  else
   value_status = 0;

  if(value_status == 1)
  {
    digitalWrite(relay_pin, LOW);
    digitalWrite(LED_BUILTIN, LOW);
    Serial.println("OFF");
  }
  else 
  {
    digitalWrite(relay_pin, HIGH);
    digitalWrite(LED_BUILTIN, HIGH);
    Serial.println("ON");
  }
  
}