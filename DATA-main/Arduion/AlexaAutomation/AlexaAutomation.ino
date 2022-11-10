#include <Arduino.h> 
#include <ESP8266WiFi.h> 
#include "SinricPro.h"
#include "SinricProSwitch.h"

#define WIFI_SSID         "eYantra"    
#define WIFI_PASS         "eyantra@bs15"
#define APP_KEY           "c880fa7b-fe5c-4d93-aa19-21382ec67657"      // App key present in the profile
#define APP_SECRET        "d0ae1867-5697-4986-a22b-ea2860fec03d-f283f703-0eb2-4cc1-8fa1-2fcad1b94a9c"   // App secret key present in the profile
#define BAUD_RATE         115200                

#define DEVICES           4                   // Number of devices
String SWITCH_IDs[DEVICES] = {                // Array of device ids to be controlled
  "61cc5d25695fce5c87534efc",
  "61cc607a0df86e5c8fef0af1",
  "61cc5cea0df86e5c8fef0920",
  "61cc5d0b695fce5c87534ed7"
};

struct{
  int deviceID;
  int deviceState;
}device;

int fan1Relay = D3;
int fan2Relay = D0;
int light1Relay = D1;
int light2Relay = D2;
int led = D5;

bool onPowerState(const String &deviceId, bool &state) {
  for (int i=0; i < DEVICES; i++) {     // go through the devices
    if (deviceId == SWITCH_IDs[i]) {    // if deviceId matches
      Serial.printf("Device number %i turned %s\r\n", i, state?"on":"off");   // print power state for device
      device.deviceID = i;
      device.deviceState = state;
    }
  }
  relaySwitch();
  return true; // request handled properly
}

// setup function for WiFi connection
void setupWiFi() {
  Serial.printf("\r\n[Wifi]: Connecting");
  WiFi.begin(WIFI_SSID, WIFI_PASS);

  while (WiFi.status() != WL_CONNECTED) {
    Serial.printf(".");
    delay(1);
    digitalWrite(led, HIGH);
    delay(100);
    digitalWrite(led, LOW);
    delay(100);
  }

  Serial.printf("connected!\r\n[WiFi]: IP-Address is %s\r\n", WiFi.localIP().toString().c_str());
}

// setup function for SinricPro
void setupSinricPro() {
  // add devices to SinricPro and set callback function
  for (int i = 0; i < DEVICES; i++) {
    SinricProSwitch& mySwitch = SinricPro[SWITCH_IDs[i]];
    mySwitch.onPowerState(onPowerState);
  }

  // setup SinricPro
  SinricPro.onConnected([](){ Serial.printf("Connected to SinricPro\r\n"); }); 
  SinricPro.onDisconnected([](){ Serial.printf("Disconnected from SinricPro\r\n"); });
  SinricPro.begin(APP_KEY, APP_SECRET);
}

void relaySwitch(){
  if(device.deviceID == 0 and device.deviceState == 1){
    Serial.println("Fan 1 on");
    digitalWrite(fan1Relay, HIGH);
  }
  else if(device.deviceID == 0 and device.deviceState == 0){
    Serial.println("Fan 1 off");
    digitalWrite(fan1Relay, LOW);
  }
  else if(device.deviceID == 1 and device.deviceState == 1){
    Serial.println("Fan 2 on");
    digitalWrite(fan2Relay, HIGH);
  }
  else if(device.deviceID == 1 and device.deviceState == 0){
    Serial.println("Fan 2 off");
    digitalWrite(fan2Relay, LOW);
  }
  else if(device.deviceID == 2 and device.deviceState == 1){
    Serial.println("Light 1 on");
    digitalWrite(light1Relay, HIGH);
  }
  else if(device.deviceID == 2 and device.deviceState == 0){
    Serial.println("Light 1 off");
    digitalWrite(light1Relay, LOW);
  }
  else if(device.deviceID == 3 and device.deviceState == 1){
    Serial.println("Light 2 on");
    digitalWrite(light2Relay, HIGH);
  }
  else if(device.deviceID == 3 and device.deviceState == 0){
    Serial.println("Light 2 off");
    digitalWrite(light2Relay, LOW);
  }
}

// main setup function
void setup() {
  Serial.begin(BAUD_RATE); Serial.printf("\r\n\r\n");

  //Setting the pins as outpyt for the relays
  pinMode(light1Relay, OUTPUT);
  pinMode(light2Relay, OUTPUT);
  pinMode(fan1Relay, OUTPUT);
  pinMode(fan2Relay, OUTPUT);
  pinMode(led, OUTPUT);
  
  //Switching them off when the microcontroller starts/restarts
  digitalWrite(light1Relay, LOW);
  digitalWrite(light2Relay, LOW);
  digitalWrite(fan1Relay, LOW);
  digitalWrite(fan2Relay, LOW);
  digitalWrite(led, LOW);
  
  setupWiFi();
  setupSinricPro();
}

void loop() {
  SinricPro.handle();             //Default function to handle the remote requests according to the functions 
}
