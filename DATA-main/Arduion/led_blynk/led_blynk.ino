#define BLYNK_TEMPLATE_ID "TMPL-tBXhYH1"
#define BLYNK_DEVICE_NAME "LED"
#define BLYNK_AUTH_TOKEN "QlXWexXc0QyUmGsIe2ueJkUcOHfpv4Xa"

#define BLYNK_PRINT Serial

#include <Blynk.h>
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

char auth[] = BLYNK_AUTH_TOKEN;
char ssid[] = "realme 5 Pro";
char pass[] = "Siddhesh@23";

void setup()
{
  Serial.begin(115200);
  //digitalWrite(0,HIGH);
  Blynk.begin(auth, ssid, pass);
}

void loop()
{
  Blynk.run();
}
