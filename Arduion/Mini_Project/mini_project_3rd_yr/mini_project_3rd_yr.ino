// libraries
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <ESP8266WebServer.h>
#include <EEPROM.h>

#include "SinricPro.h"
#include "SinricProSwitch.h"

#include <map>

//#define WIFI_SSID         "1234"
//#define WIFI_PASS         "Yash#1290"

#define APP_KEY "2a072e9e-eee8-4eb1-a5ec-4cea1c92fefd"                                          // Should look like "de0bxxxx-1x3x-4x3x-ax2x-5dabxxxxxxxx"
#define APP_SECRET "e7a7a739-0530-4fcf-a248-73484036ee51-d5c850c7-eb85-4f1a-9f46-c59ff54e29a4"  // Should look like "5f36xxxx-x3x7-4x3x-xexe-e86724a9xxxx-4c4axxxx-3x3x-x5xe-x9x3-333d65xxxxxx"

#define BAUD_RATE   115200

#define DEBOUNCE_TIME 250

#define buzzer_pin 16 // D0

#define RELAYPIN_1 5
#define RELAYPIN_2 4
#define RELAYPIN_3 2

int statusCode;
String content;

ESP8266WebServer server(80);

typedef struct {      // struct for the std::map below
  int relayPIN;
  int flipSwitchPIN;
  bool activeLow;
} deviceConfig_t;

std::map<String, deviceConfig_t> devices = {
  // deviceid , relaypin,switchpin,activelow_status
  { "636b7d4fb8a7fefbd6368a59", {  5, 14, true } },   //  D1  D5
  { "636b7e13b8a7fefbd6368af3", {  4, 12, true } },   //  D2  D6
  { "636b7e32b8a7fefbd6368b3b", {  2, 13, true } },  //  D8  D7
};

typedef struct {      // struct for the std::map below
  String deviceId;
  bool lastFlipSwitchState;
  unsigned long lastFlipSwitchChange;
  bool activeLow;
} flipSwitchConfig_t;

std::map<int, flipSwitchConfig_t> flipSwitches;

void setupRelays() { 
  for (auto &device : devices) {           // for each device (relay, flipSwitch combination)
    int relayPIN = device.second.relayPIN; // get the relay pin
    pinMode(relayPIN, OUTPUT);             // set relay pin to OUTPUT
    
  }
}

void setupFlipSwitches() {
  for (auto &device : devices)  {                     // for each device (relay / flipSwitch combination)
    flipSwitchConfig_t flipSwitchConfig;              // create a new flipSwitch configuration

    flipSwitchConfig.deviceId = device.first;         // set the deviceId
    flipSwitchConfig.lastFlipSwitchChange = 0;        // set debounce time
    flipSwitchConfig.lastFlipSwitchState = false;     // set lastFlipSwitchState to false (LOW)
    int flipSwitchPIN = device.second.flipSwitchPIN;  // get the flipSwitchPIN
    bool activeLow = device.second.activeLow;         // set the activeLow
    flipSwitchConfig.activeLow = activeLow;           
    flipSwitches[flipSwitchPIN] = flipSwitchConfig;   // save the flipSwitch config to flipSwitches map
    
    if(activeLow) {
      pinMode(flipSwitchPIN, INPUT_PULLUP);           // set the flipSwitch pin to INPUT_PULLUP
    }
    else {
      pinMode(flipSwitchPIN, INPUT);                  // set the flipSwitch pin to INPUT  
    } 
  }
}

bool onPowerState(String deviceId, bool &state) {
  Serial.printf("%s: %s\r\n", deviceId.c_str(), state ? "on" : "off");
  int relayPIN = devices[deviceId].relayPIN; // get the relay pin for corresponding device
  digitalWrite(relayPIN, state);             // set the new relay state
  return true;
}

void handleFlipSwitches() {
  unsigned long actualMillis = millis();                                          // get actual millis
  for (auto &flipSwitch : flipSwitches) {                                         // for each flipSwitch in flipSwitches map
    unsigned long lastFlipSwitchChange = flipSwitch.second.lastFlipSwitchChange;  // get the timestamp when flipSwitch was pressed last time (used to debounce / limit events)

    if (actualMillis - lastFlipSwitchChange > DEBOUNCE_TIME) {                    // if time is > debounce time...

      int flipSwitchPIN = flipSwitch.first;                                       // get the flipSwitch pin from configuration
      bool lastFlipSwitchState = flipSwitch.second.lastFlipSwitchState;           // get the lastFlipSwitchState
      bool activeLow = flipSwitch.second.activeLow; 
      bool flipSwitchState = digitalRead(flipSwitchPIN);                          // read the current flipSwitch state
      if(activeLow) flipSwitchState = !flipSwitchState;

      if (flipSwitchState != lastFlipSwitchState) {                               // if the flipSwitchState has changed...     
          flipSwitch.second.lastFlipSwitchChange = actualMillis;                  // update lastFlipSwitchChange time
          String deviceId = flipSwitch.second.deviceId;                           // get the deviceId from config
          int relayPIN = devices[deviceId].relayPIN;                              // get the relayPIN from config
          bool newRelayState = !digitalRead(relayPIN);                            // set the new relay State
          digitalWrite(relayPIN, newRelayState);                                  // set the trelay to the new state

          SinricProSwitch &mySwitch = SinricPro[deviceId];                        // get Switch device from SinricPro
          mySwitch.sendPowerStateEvent(newRelayState);                            // send the event     
        flipSwitch.second.lastFlipSwitchState = flipSwitchState;                  // update lastFlipSwitchState
      }
    }
  }
}

void setupWifi(){
  Serial.println("Disconnecting previously connected WiFi");
  WiFi.disconnect();
  EEPROM.begin(512); //Initialasing EEPROM with 512 bytes storage capacity
  delay(10);
  Serial.println();
  Serial.println();
  Serial.println("Startup");
 
  //---------------------------------------- Read EEPROM for SSID and pass
  Serial.println("Reading EEPROM ssid");
 
  String esid;
  for (int i = 0; i < 32; ++i)
  {
    esid += char(EEPROM.read(i));
  }
  Serial.println();
  Serial.print("SSID: ");
  Serial.println(esid);
  Serial.println("Reading EEPROM pass");
 
  String epass = "";
  for (int i = 32; i < 96; ++i)
  {
    epass += char(EEPROM.read(i));
  }
  Serial.print("PASS: ");
  Serial.println(epass);
 
 
  WiFi.begin(esid.c_str(), epass.c_str());
  if (testWifi())
  {
    
    Serial.println("Succesfully Connected!!!");
    digitalWrite(buzzer_pin,LOW);
    return;
  }
  else
  {
    Serial.println("Turning the HotSpot On");
    digitalWrite(buzzer_pin,HIGH);
    launchWeb();
    setupAP();// Setup HotSpot
  }
 
  Serial.println();
  Serial.println("Waiting.");
  
  while ((WiFi.status() != WL_CONNECTED))
  {
    Serial.print(".");
    delay(1000);
    server.handleClient();
  }
}

bool testWifi(void)
{
  int c = 0;
  Serial.println("Waiting for Wifi to connect");
  while ( c < 20 ) {
    //digitalWrite(LED_BUILTIN,LOW);
    if (WiFi.status() == WL_CONNECTED)
    {
      return true;
    }
    delay(500);
    Serial.print("*");
    c++;
    //digitalWrite(LED_BUILTIN,HIGH);
  }
  Serial.println("");
  Serial.println("Connect timed out, opening AP");
  return false;
}

void launchWeb()
{
  
  Serial.println("");
  if (WiFi.status() == WL_CONNECTED)
  {
    Serial.println("WiFi connected");
    digitalWrite(buzzer_pin,LOW);
    delay(5);
  }
  Serial.print("Local IP: ");
  Serial.println(WiFi.localIP());
  Serial.print("SoftAP IP: ");
  Serial.println(WiFi.softAPIP());
  createWebServer();
  // Start the server
  server.begin();
  Serial.println("Server started");
}

void setupAP(void)
{
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  delay(100);
  
  Serial.println("");
  delay(100);
  WiFi.softAP("IoT_Switch","123456780");
  Serial.println("softap");
  launchWeb();
  Serial.println("over");
 
}

//html
void createWebServer()
{
  digitalWrite(buzzer_pin,HIGH);
  delay(100);
 {
    server.on("/", []() {
      IPAddress ip = WiFi.softAPIP();
      String ipStr = String(ip[0]) + '.' + String(ip[1]) + '.' + String(ip[2]) + '.' + String(ip[3]);
      content = "<!DOCTYPE HTML>\r\n<html><head><title>AP MODE</title></head>";
      content += "<body><h1>Ap Mode</h1><form method='get' action='setting'><label>SSID: </label><input name='ssid' length=32><br></br><label>PASS: </label><input name='pass' length=64><br></br><input type='submit' value='SUBMIT'></form></body>";
      content += "</html>";
      server.send(200, "text/html", content);
    });
    server.on("/setting", []() {
      String qsid = server.arg("ssid");
      String qpass = server.arg("pass");
      if (qsid.length() > 0 && qpass.length() > 0) {
        Serial.println("clearing eeprom");
        for (int i = 0; i < 96; ++i) {
          EEPROM.write(i, 0);
        }
        Serial.println(qsid);
        Serial.println("");
        Serial.println(qpass);
        Serial.println("");
        Serial.println("writing eeprom ssid:");
        for (int i = 0; i < qsid.length(); ++i)
        {
          EEPROM.write(i, qsid[i]);
          Serial.print("Wrote: ");
          Serial.println(qsid[i]);
        }
        Serial.println("writing eeprom pass:");
        for (int i = 0; i < qpass.length(); ++i)
        {
          EEPROM.write(32 + i, qpass[i]);
          Serial.print("Wrote: ");
          Serial.println(qpass[i]);
        }
        EEPROM.commit();
        content = "{\"Success\":\"saved to eeprom... reset to boot into new wifi\"}";
        statusCode = 200;
        ESP.reset();
      } else {
        content = "{\"Error\":\"404 not found\"}";
        statusCode = 404;
        Serial.println("Sending 404");
      }
      server.sendHeader("Access-Control-Allow-Origin", "*");
      server.send(statusCode, "application/json", content);
    });
  }
}

void setupSinricPro() {
  
  for (auto &device : devices) {
    const char *deviceId = device.first.c_str();
    SinricProSwitch &mySwitch = SinricPro[deviceId];
    mySwitch.onPowerState(onPowerState);
  }

  SinricPro.restoreDeviceStates(true); // restore the last known state from the server.
  
  SinricPro.begin(APP_KEY, APP_SECRET);  
}

void setup() {
  Serial.begin(BAUD_RATE);
  pinMode(buzzer_pin,OUTPUT);
  digitalWrite(buzzer_pin,LOW);
  setupRelays();
  setupFlipSwitches();
  setupWifi();
  setupSinricPro();
}

void loop() {
  SinricPro.handle();
  handleFlipSwitches();
}
