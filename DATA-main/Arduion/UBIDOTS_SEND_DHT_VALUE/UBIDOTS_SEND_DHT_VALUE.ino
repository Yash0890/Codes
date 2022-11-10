#include "UbidotsMicroESP8266.h"
#include "DHT.h"
#define TOKEN  "A1E-WUG6CI5HcGpFinHs6JDlWkHg7VCDPKmXygLv1oxLNdRWwkEDP75TZy0H"  // Put here your Ubidots TOKEN OF APP OR API TOKEN
#define WIFISSID "Redmi"
#define PASSWORD "mayur123"

Ubidots client(TOKEN);

//#define DHTPIN D1 // what pin we're connected to

// DHT Sensor
const int DHTPin = 5;    //gpio pin


#define DHTTYPE DHT11   // DHT 11
float hum;
float temp;
 
// Initialize DHT sensor.
DHT dht(DHTPin, DHTTYPE);

void setup(){
    Serial.begin(9600);
    delay(10);
    dht.begin();
    client.wifiConnection(WIFISSID, PASSWORD);
}
void loop()
{

  for (int i=0;i<50;i++)
  {
  // Reading temperature or humidity takes about 250 milliseconds!
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
        hum += dht.readHumidity();
  // Read temperature as Celsius (the default)
        temp += dht.readTemperature();
        delay(10);
 // Check if any reads failed and exit early (to try again).
  if (isnan(hum) || isnan(temp)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  }
   hum = hum/50;
   temp = temp/50;
    {
    client.add("LAB TEMPERATURE",temp);
    client.add("LAB HUMIDITY",hum);
    client.sendAll(true);
    delay(1000);
    }
}
