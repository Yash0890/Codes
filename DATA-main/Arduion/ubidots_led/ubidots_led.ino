
#include "UbidotsMicroESP8266.h"

#define ID  "5ce660adc03f974d6b584d15"  // Put here your Ubidots variable ID
//#define ID2  "595e1204762542310acfe1ba"  // Put here your Ubidots variable ID
#define TOKEN  "A1E-WUG6CI5HcGpFinHs6JDlWkHg7VCDPKmXygLv1oxLNdRWwkEDP75TZy0H"  // Put here your Ubidots TOKEN
#define WIFISSID "Redmi" // Put here your Wi-Fi SSID
#define PASSWORD "mayur123" // Put here your Wi-Fi password

Ubidots client(TOKEN);

void setup() {
    pinMode(16,OUTPUT);
    digitalWrite(16,LOW);
   // pinMode(D7,OUTPUT);
    //digitalWrite(D7,LOW);
    Serial.begin(9600);
    client.wifiConnection(WIFISSID, PASSWORD);
    //client.setDebug(true); // Uncomment this line to set DEBUG on
}

void loop() {
    int value = client.getValue(ID);
    //int value2 = client.getValue(ID2);
    digitalWrite(16,value);
    //digitalWrite(D7,value2);
    Serial.print("Value: ");
    Serial.print(value);
   // Serial.print("Value2: ");
    //Serial.println(value2);
    delay(100);
}
