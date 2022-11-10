// lib
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <ESP8266WebServer.h>
#include <EEPROM.h>

#include "SinricPro.h"
#include "SinricProSwitch.h"

#include <map>

//##################################################################################################################//
// sinric details
#define APP_KEY           "2a072e9e-eee8-4eb1-a5ec-4cea1c92fefd"
#define APP_SECRET        "e7a7a739-0530-4fcf-a248-73484036ee51-d5c850c7-eb85-4f1a-9f46-c59ff54e29a4"

#define BAUD_RATE 115200

#define DEBOUNCE_TIME 250

// realy pins
#define switch1_pin 1
#define switch2_pin 3
#define switch3_pin 5

//##################################################################################################################//

// device id structure
typedef struct {      // struct for the std::map below
  int relayPIN;
  int flipSwitchPIN;
  bool activeLow;
} deviceConfig_t;

std::map<String, deviceConfig_t> devices = {
    //{deviceId, {relayPIN,  flipSwitchPIN, activeLow}}
    {"636b7d4fb8a7fefbd6368a59", {  1, 4, false }},
    {"636b7e13b8a7fefbd6368af3", {  3, 0, false }},
    {"636b7e32b8a7fefbd6368b3b", {  5, 2, false }},     
};

//*****************************************************************//
// manual switch pressed structure
typedef struct {      // struct for the std::map below
  String deviceId;
  bool lastFlipSwitchState;
  unsigned long lastFlipSwitchChange;
  bool activeLow;
} flipSwitchConfig_t;

std::map<int, flipSwitchConfig_t> flipSwitches;

//##################################################################################################################//

void setupRelays() { 
  for (auto &device : devices) {           // for each device (relay, flipSwitch combination)
    int relayPIN = device.second.relayPIN; // get the relay pin
    pinMode(relayPIN, OUTPUT);             // set relay pin to OUTPUT
  }
}




















