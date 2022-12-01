#include "DHT.h"
#include <WiFi.h>
#include "time.h"

// Time 
const char* ntpServer = "pool.ntp.org";
const long  gmtOffset_sec = 19800;
const int   daylightOffset_sec = 3600;

// DHT PIN
#define DHTPIN 4 
#define DHTTYPE DHT22

// Soil Moisture PIN
#define AOUT_PIN1 36 
#define AOUT_PIN2 39
#define AOUT_PIN3 34

// Wifi Details 
const char* ssid = "12345";
const char* password = "Yash#1291";

// DHT init
DHT dht(DHTPIN, DHTTYPE);

// Server init
WiFiServer server(80);

void setup() 
{
  Serial.begin(115200);

  //DHT init
  dht.begin();

  // Connect to WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);     //Begin WiFi

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");

  // Get Time & Date
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
  get_LocalTime();

  // Start the server
  server.begin();
  Serial.println("Server started");

  // Print the IP address on serial monitor
  Serial.print("Use this URL to connect: ");
  Serial.print("http://");    //URL IP to be typed in mobile/desktop browser
  //Serial.print(WiFi.localIP());
  Serial.println("/");

}

void loop() {

  delay(2000);

  // Read Humidity 
  float h = dht.readHumidity();
  
  // Read temperature as Celsius (the default)
  float t = dht.readTemperature();
  
  // Read temperature as Fahrenheit (isFahrenheit = true)
  float f = dht.readTemperature(true);

  // Plant 1
  int value1 = analogRead(AOUT_PIN1); 

  // Plant 2
  int value2 = analogRead(AOUT_PIN2); 

  //Plant 3
  int value3 = analogRead(AOUT_PIN3); 


  // Check if any reads failed and exit early (to try again).
  if (isnan(h) || isnan(t) || isnan(f)) {
    Serial.println(F("Failed to read from DHT sensor!"));
    return;
  }

  char val[100];
  sprintf(val,"\n\n\n\nMoisture value \nSensor 1: %d\n\nSensor 2: %d\n\nSensor 3: %d",value1,value2,value3);
  Serial.println(val);

  char temp_val[100];
  sprintf(temp_val,"Humidity : %d %\nTemperature : %d °C",h,t);
  Serial.println(temp_val);

}

void get_LocalTime(){
  struct tm timeinfo;
  if(!getLocalTime(&timeinfo)){
    Serial.println("Failed to obtain time");
    return;
  }
  Serial.println(&timeinfo, "%A, %B %d %Y %H:%M:%S");
 Serial.println();
}