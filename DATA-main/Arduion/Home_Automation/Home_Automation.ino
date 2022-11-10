// libraries
#include <ESP8266WiFi.h>
#include "SinricPro.h"
#include "SinricProSwitch.h"
#include <ESP8266HTTPClient.h>
#include <ESP8266WebServer.h>
#include <EEPROM.h>

#define APP_KEY           "f5bfa38c-01a1-4315-abbc-544b04d87d7b"
#define APP_SECRET        "6e6a8454-df25-4103-bef3-8f4d8ba8a92f-6f382ce9-6754-4bd7-a3a5-0911fd7e5e42"

#define DEVICES           3                   // Number of devices
String SWITCH_IDs[DEVICES] = {                // Array of device ids to be controlled
  "62e7ff364dd95ec7bdbe068e", // Light_pin_1
  "62e7ff940bb10053866912f5", // Light_pin_2
  "61cc5cea0df86e5c8fef0920", // Fan_pin_1
};

struct{
  int deviceID;
  int deviceState;
}device;

int i = 0;
int statusCode;

int Light_Pin_1 = D1;
int Light_Pin_2 = D2;
int Fan_Pin_2 = D3;
int Fan_Pin_1 = D0;

int Wifi_led = D4;
int AP_mode_led = D5;

String content;
bool myPowerState = false;

//Function Decalration
bool testWifi(void);
void launchWeb(void);
void setupAP(void);

ESP8266WebServer server(80);

bool onPowerState(const String &deviceId, bool &state) {
  for (int i = 0; i < DEVICES; i++) {   // go through the devices
    if (deviceId == SWITCH_IDs[i]) {    // if deviceId matches
      Serial.printf("Device number %i turned %s\r\n", i, state ? "on" : "off"); // print power state for device
      device.deviceID = i;
      device.deviceState = state;
    }
  }
  relaySwitch();
  return true; // request handled properly
}

// starting sinric pro
void setupSinricPro() {
  for (int i = 0; i < DEVICES ; i++)
  {
    // add device to SinricPro
    SinricProSwitch& mySwitch = SinricPro[SWITCH_IDs[i]];
    // set callback function to device
    mySwitch.onPowerState(onPowerState);
  }
  // setup SinricPro
  SinricPro.onConnected([]() {
    Serial.printf("Connected to SinricPro\r\n");
  });
  SinricPro.onDisconnected([]() {
    Serial.printf("Disconnected from SinricPro\r\n");
  });
  SinricPro.begin(APP_KEY, APP_SECRET);
}

// Relay Controll
void relaySwitch() {
  if (device.deviceID == 0 and device.deviceState == 1) {
    Serial.println("Fan 1 on");
    digitalWrite(Fan_Pin_1, HIGH);
  }
  else if (device.deviceID == 0 and device.deviceState == 0) {
    Serial.println("Fan 1 off");
    digitalWrite(Fan_Pin_1, LOW);
  }
  else if (device.deviceID == 1 and device.deviceState == 1) {
    Serial.println("Fan 2 on");
    digitalWrite(Fan_Pin_2, HIGH);
  }
  else if (device.deviceID == 1 and device.deviceState == 0) {
    Serial.println("Fan 2 off");
    digitalWrite(Fan_Pin_2, LOW);
  }
  else if (device.deviceID == 2 and device.deviceState == 1) {
    Serial.println("Light 1 on");
    digitalWrite(Light_Pin_1, HIGH);
  }
  else if (device.deviceID == 2 and device.deviceState == 0) {
    Serial.println("Light 1 off");
    digitalWrite(Light_Pin_1, LOW);
  }
  else if (device.deviceID == 3 and device.deviceState == 1) {
    Serial.println("Light 2 on");
    digitalWrite(Light_Pin_2, HIGH);
  }
  else if (device.deviceID == 3 and device.deviceState == 0) {
    Serial.println("Light 2 off");
    digitalWrite(Light_Pin_2, LOW);
  }
}

// wifi setup
void setupWifi() {
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
  for (i = 0; i < 32; ++i)
  {
    esid += char(EEPROM.read(i));
  }
  Serial.println();
  Serial.print("SSID: ");
  Serial.println(esid);
  Serial.println("Reading EEPROM pass");

  String epass = "";
  for (i = 32; i < 96; ++i)
  {
    epass += char(EEPROM.read(i));
  }
  Serial.print("PASS: ");
  Serial.println(epass);
  
  WiFi.begin(esid.c_str(), epass.c_str());
  if (testWifi())
  {

    Serial.println("Succesfully Connected!!!");
    return;
  }
  else
  {
    Serial.println("Turning the HotSpot On");
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

void setup()
{

  Serial.begin(9600); //Initialising if(DEBUG)Serial Monitor
  Serial.println();
  pinMode(Light_Pin_1, OUTPUT);
  pinMode(Light_Pin_2, OUTPUT);
  pinMode(Fan_Pin_1, OUTPUT);
  pinMode(Fan_Pin_2, OUTPUT);
  pinMode(Wifi_led, OUTPUT);
  pinMode(AP_mode_led, OUTPUT);

  //Switching them off when the microcontroller starts/restarts
  digitalWrite(Light_Pin_1, LOW);
  digitalWrite(Light_Pin_2, LOW);
  digitalWrite(Fan_Pin_1, LOW);
  digitalWrite(Fan_Pin_2, LOW);

  setupWifi();
  setupSinricPro();
}

void loop() {
  SinricPro.handle();
}

// checking previous wifi network
bool testWifi(void)
{
  int c = 0;
  Serial.println("Waiting for Wifi to connect");
  while ( c < 20 ) {
    if (WiFi.status() == WL_CONNECTED)
    {
      for(int x;x<=10;x++)
      {
        digitalWrite(Wifi_led,HIGH);
        delay(10);
        digitalWrite(Wifi_led,LOW);
        delay(10);
      }
      return true;
    }
    delay(500);
    Serial.print("*");
    c++;
  }
  Serial.println("");
  Serial.println("Connect timed out, opening AP");
  return false;
}

// softap ip startup
void launchWeb()
{
  Serial.println("");
  if (WiFi.status() == WL_CONNECTED)
    Serial.println("WiFi connected");
  Serial.print("Local IP: ");
  Serial.println(WiFi.localIP());
  Serial.print("SoftAP IP: ");
  Serial.println(WiFi.softAPIP());
  createWebServer();
  // Start the server
  server.begin();
  Serial.println("Server started");
}

//ap mode setup
void setupAP(void)
{
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  delay(90);
  for(int x;x<=10;x++)
  {
    digitalWrite(AP_mode_led,HIGH);
    delay(10);
    digitalWrite(AP_mode_led,LOW);
    delay(10);
  }
  Serial.println("");
  delay(100);
  WiFi.softAP("IoT_Switch", "123456780");
  Serial.println("softap");
  launchWeb();
  Serial.println("over");
}

//html
void createWebServer()
{
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
        for (i = 0; i < 96; ++i) {
          EEPROM.write(i, 0);
        }
        Serial.println(qsid);
        Serial.println("");
        Serial.println(qpass);
        Serial.println("");
        Serial.println("writing eeprom ssid:");
        for (i = 0; i < qsid.length(); ++i)
        {
          EEPROM.write(i, qsid[i]);
          Serial.print("Wrote: ");
          Serial.println(qsid[i]);
        }
        Serial.println("writing eeprom pass:");
        for (i = 0; i < qpass.length(); ++i)
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
