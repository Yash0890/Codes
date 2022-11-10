// Import required libraries
#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>

const char* ssid     = "ACBD";
const char* password = "123456789";

#define MOTORPIN1 26
#define MOTORPIN1 27              // Digital pin connected to the DHT sensor

const char* PARAM_INPUT_1 = "output";
const char* PARAM_INPUT_2 = "state";

// Create AsyncWebServer object on port 80
AsyncWebServer server(80);

// Generally, you should use "unsigned long" for variables that hold time
// The value will quickly become too large for an int to store
unsigned long previousMillis = 0;    // will store last time DHT was updated

// Updates DHT readings every 10 seconds
const long interval = 10000;  

const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML><html><head>
    <title>ESP Input Form</title>
    <meta name="viewport" content="width=device-width, initial-scale=1">
    <link rel="icon" href="data:,">
    <style>
        html {
            font-family: Arial;
            display: inline-block;
            margin: 0px auto;
            text-align: center;
        }
        h2 { 
            font-size: 3.0rem; 
        }
        p { 
            font-size: 3.0rem; 
        }
        .units { 
            font-size: 1.2rem; 
        }
        .dht-labels{
            font-size: 1.5rem;
            vertical-align:middle;
            padding-bottom: 15px;
        }
        body {
            max-width: 600px; 
            margin:0px auto; 
            padding-bottom: 25px;
        }
        .switch {
            position: relative; 
            display: inline-block; 
            width: 120px; 
            height: 68px
        } 
        .switch input {
            display: none
        }
        .slider {
            position: absolute; 
            top: 0; 
            left: 0; 
            right: 0; 
            bottom: 0; 
            background-color: #ccc; 
            border-radius: 6px
        }
        .slider:before {
            position: absolute; 
            content: ""; 
            height: 52px; 
            width: 52px; 
            left: 8px; 
            bottom: 8px; 
            background-color: #fff; 
            -webkit-transition: .4s; 
            transition: .4s; 
            border-radius: 3px
        }
        input:checked+.slider {
            background-color: #b30000
        }
        input:checked+.slider:before {
            -webkit-transform: translateX(52px); 
            -ms-transform: translateX(52px); 
            transform: translateX(52px)
        }
    </style>
    </head><body>
    

  %BUTTONPLACEHOLDER%
  <script>function toggleCheckbox(element) {
    var xhr = new XMLHttpRequest();
    if(element.checked){ xhr.open("GET", "/get?output="+element.id+"&state=1", true); }
    else { xhr.open("GET", "/get?output="+element.id+"&state=0", true); }
    xhr.send();
}
</script>
</body></html>)rawliteral";

String processor(const String& var) {
  if (var == "BUTTONPLACEHOLDER") {
    String buttons = "";
    buttons += "<h4>Clockwise</h4><label class=\"switch\"><input type=\"checkbox\" onchange=\"toggleCheckbox(this)\" id=\"Clockwise\" " + outputState(2) + "><span class=\"slider\"></span></label>";
    buttons += "<h4>AntiClockwise</h4><label class=\"switch\"><input type=\"checkbox\" onchange=\"toggleCheckbox(this)\" id=\"Anticlockwise\" " + outputState(23) + "><span class=\"slider\"></span></label>";
    return buttons;
  }
}

String outputState(int output) {
  if (digitalRead(output)) {
    return "checked";
  }
  else {
    return "";
  }
}

void setup(){
  // Serial port for debugging purposes
  Serial.begin(115200);
  pinMode(MOTORPIN1, OUTPUT);
  pinMode(MOTORPIN2, OUTPUT);
  
  Serial.print("Setting AP (Access Point)…");
  WiFi.softAP(ssid, password);
  delay(100);

  IPAddress Ip(192, 168, 4, 22);
  IPAddress NMask(255, 255, 255, 0);
  WiFi.softAPConfig(Ip, Ip, NMask);

  IPAddress myIP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(myIP);

  // Print ESP8266 Local IP Address
  Serial.println(WiFi.localIP());

server.on("/get", HTTP_GET, [] (AsyncWebServerRequest * request) {
    String inputMessage1;
    String inputMessage2;
    // GET input1 value on <ESP_IP>/update?output=<inputMessage1>&state=<inputMessage2>
    if (request->hasParam(PARAM_INPUT_1) && request->hasParam(PARAM_INPUT_2)) {
      inputMessage1 = request->getParam(PARAM_INPUT_1)->value();
      inputMessage2 = request->getParam(PARAM_INPUT_2)->value();

      if (inputMessage1 == "Clockwise") {
        if (inputMessage2 == "1") {
          digitalWrite(MOTORPIN1, HIGH);
          digitalWrite(MOTORPIN2, LOW);
        }
        else {
          digitalWrite(MOTORPIN1, LOW);
          digitalWrite(MOTORPIN2, LOW);
        }
      }
      else if (inputMessage1 == "Anticlockwise") {
        if (inputMessage2 == "1") {
          digitalWrite(MOTORPIN1, LOW);
          digitalWrite(MOTORPIN2, HIGH);
        }
        else {
          digitalWrite(MOTORPIN1, LOW);
          digitalWrite(MOTORPIN2, LOW);
        }
      }
    }
    else {
      inputMessage1 = "No message sent";
      inputMessage2 = "No message sent";
      delay(50);
    }
    Serial.print("GPIO: ");
    Serial.print(inputMessage1);
    Serial.print(" - Set to: ");
    Serial.println(inputMessage2);
    request->send(200, "text/plain", "OK");
    delay(20);
    //    Serial.println("End");
  });


  // Start server
  server.begin();
}
 
void loop(){  
  unsigned long currentMillis = millis();
  
}
