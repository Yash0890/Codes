#include <SPI.h>              // include libraries
#include <LoRa.h>
#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>

const int csPin = 18;          // LoRa radio chip select
const int resetPin = 14;       // LoRa radio reset
const int irqPin = 26;         // change for your board; must be a hardware interrupt pin

// Replace with your network credentials
const char* ssid = "Greenhouse";
const char* password = "123456789";

const char* PARAM_INPUT_MIN_TEMP = "mintemp";
const char* PARAM_INPUT_MAX_TEMP = "maxtemp";
const char* PARAM_INPUT_IRRI_TIMER = "timer";

const char* PARAM_INPUT_1 = "output";
const char* PARAM_INPUT_2 = "state";

String outgoing;              // outgoing message

byte msgCount = 0;            // count of outgoing messages
byte localAddress = 0x0A;     // address of this device
byte destination;      // destination to send to
long lastSendTime = 0;        // last send time
int interval = 2000;          // interval between sends
bool flag = false;

String Temperature;
String Humidity;

bool heaterAutomaticSwitchOn = false;
bool fanAutomaticSwitchOn = false;
bool solenoidAutomaticSwitchOn = false;

AsyncWebServer server(80);

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
    <p>
        <i class="fas fa-thermometer-half" style="color:#059e8a;"></i> 
        <span class="dht-labels">Temperature</span> 
        <span id="temperature">%TEMPERATURE%</span>
        <sup class="units">&deg;C</sup>
    </p>
    <p>
        <i class="fas fa-tint" style="color:#00add6;"></i> 
        <span class="dht-labels">Humidity</span>
        <span id="humidity">%HUMIDITY%</span>
        <sup class="units">&percnt;</sup>
    </p>
    <script>
        setInterval(function ( ) {
            var xhttp = new XMLHttpRequest();
            xhttp.onreadystatechange = function() {
            if (this.readyState == 4 && this.status == 200) {
                document.getElementById("temperature").innerHTML = this.responseText;
            }
        };
        xhttp.open("GET", "/temperature", true);
        xhttp.send();
    }, 10000 ) ;

        setInterval(function ( ) {
            var xhttp = new XMLHttpRequest();
            xhttp.onreadystatechange = function() {
            if (this.readyState == 4 && this.status == 200) {
                document.getElementById("humidity").innerHTML = this.responseText;
            }
        };
        xhttp.open("GET", "/humidity", true);
        xhttp.send();
    }, 10000 ) ;
    </script>

  %BUTTONPLACEHOLDER%
  <script>function toggleCheckbox(element) {
    var xhr = new XMLHttpRequest();
    if(element.checked){ xhr.open("GET", "/get?output="+element.id+"&state=1", true); }
    else { xhr.open("GET", "/get?output="+element.id+"&state=0", true); }
    xhr.send();
}
</script>
    <form action="/minTemp">
        Min. Temp: <input type="text" name="mintemp">
        <input type="submit" value="Submit">
    </form><br>
    <form action="/maxTemp">
        Max. Temp: <input type="text" name="maxtemp">
        <input type="submit" value="Submit">
    </form><br>
    <form action="/timer">
        Irrigation Timer: <input type="text" name="timer">
        <input type="submit" value="Submit">
    </form><br>
</body></html>)rawliteral";

String processor(const String& var) {
  if (var == "BUTTONPLACEHOLDER") {
    String buttons = "";
    buttons += "<h4>Irrigation System</h4><label class=\"switch\"><input type=\"checkbox\" onchange=\"toggleCheckbox(this)\" id=\"Solenoid\" " + outputState(2) + "><span class=\"slider\"></span></label>";
    buttons += "<h4>Fan</h4><label class=\"switch\"><input type=\"checkbox\" onchange=\"toggleCheckbox(this)\" id=\"Fan\" " + outputState(23) + "><span class=\"slider\"></span></label>";
    buttons += "<h4>Heater</h4><label class=\"switch\"><input type=\"checkbox\" onchange=\"toggleCheckbox(this)\" id=\"Heater\" " + outputState(22) + "><span class=\"slider\"></span></label>";
    return buttons;
  }
  else if (var == "TEMPERATURE") {
    return Temperature;
  }
  else if (var == "HUMIDITY") {
    return Humidity;
  }
  return String();
}

String outputState(int output) {
  if (digitalRead(output)) {
    return "checked";
  }
  else {
    return "";
  }
}

void setup() {
  Serial.begin(9600);                   // initialize serial
  while (!Serial);

  Serial.print("Setting AP (Access Point)…");
  WiFi.softAP(ssid, password);
  delay(100);

  IPAddress Ip(192, 168, 4, 22);
  IPAddress NMask(255, 255, 255, 0);
  WiFi.softAPConfig(Ip, Ip, NMask);

  IPAddress myIP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(myIP);

  Serial.println("LoRa Duplex");

  // override the default CS, reset, and IRQ pins (optional)
  LoRa.setPins(csPin, resetPin, irqPin);// set CS, reset, IRQ pin

  if (!LoRa.begin(433E6)) {             // initialize ratio at 915 MHz
    Serial.println("LoRa init failed. Check your connections.");
    while (true);                       // if failed, do nothing
  }

  Serial.println("LoRa init succeeded.");
  server.on("/", HTTP_GET, [](AsyncWebServerRequest * request) {
    request->send_P(200, "text/html", index_html, processor);
    delay(20);
  });

  server.on("/get", HTTP_GET, [] (AsyncWebServerRequest * request) {
    String inputMessage1;
    String inputMessage2;
    // GET input1 value on <ESP_IP>/update?output=<inputMessage1>&state=<inputMessage2>
    if (request->hasParam(PARAM_INPUT_1) && request->hasParam(PARAM_INPUT_2)) {
      inputMessage1 = request->getParam(PARAM_INPUT_1)->value();
      inputMessage2 = request->getParam(PARAM_INPUT_2)->value();

      if (inputMessage1 == "Solenoid") {
        if (inputMessage2 == "1") {
          destination = 0xAC;
          Serial.println("Sending Solenoid");
          sendMessage("1#Sol");
          delay(50);
        }
        else {
          destination = 0xAC;
          Serial.println("Sending Solenoid");
          sendMessage("2#Sol");
          delay(50);
        }
      }
      else if (inputMessage1 == "Fan") {
        if (inputMessage2 == "1") {
          destination = 0xAB;
          sendMessage("3");
          delay(50);
        }
        else {
          destination = 0xAB;
          sendMessage("4");
          delay(50);
        }
      }
      else if (inputMessage1 == "Heater") {
        if (inputMessage2 == "1") {
          destination = 0xAB;
          sendMessage("5");
          delay(50);
        }
        else {
          destination = 0xAB;
          sendMessage("6");
          delay(50);
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

  server.on("/maxTemp", HTTP_GET, [](AsyncWebServerRequest * request) {
    String inputMessage;
    String inputParam;
    String message;

    if (request->hasParam(PARAM_INPUT_MAX_TEMP)) {
      inputMessage = request->getParam(PARAM_INPUT_MAX_TEMP)->value();
      inputParam = PARAM_INPUT_2;
      Serial.println(inputMessage);
      destination = 0xAB;
      message = inputMessage + "#max";
      if (inputMessage.toInt() < 100 && inputMessage.toInt() > 0)
        sendMessage(message);
      delay(100);
    }
    else {
      inputMessage = "No message sent";
      inputParam = "none";
    }

    request->send(200, "text/html", "Maximum Temperature for System set to: "
                  + inputMessage +
                  "<br><a href=\"/\">Return to Home Page</a>");
    delay(20);
  });

  server.on("/minTemp", HTTP_GET, [](AsyncWebServerRequest * request) {
    String inputMessage;
    String inputParam;
    String message;

    if (request->hasParam(PARAM_INPUT_MIN_TEMP)) {
      inputMessage = request->getParam(PARAM_INPUT_MIN_TEMP)->value();
      inputParam = PARAM_INPUT_1;
      Serial.println(inputMessage);
      destination = 0xAB;
      message = inputMessage + "#min";
      if (inputMessage.toInt() < 100 && inputMessage.toInt() > 0)
        sendMessage(message);
      delay(100);
    }
    else {
      inputMessage = "No message sent";
      inputParam = "none";
    }

    request->send(200, "text/html", "Minimum Temperature for System set to: "
                  + inputMessage +
                  "<br><a href=\"/\">Return to Home Page</a>");
    delay(20);
  });

  server.on("/temperature", HTTP_GET, [](AsyncWebServerRequest * request) {
    request->send_P(200, "text/plain", Temperature.c_str());
    delay(20);
  });

  server.on("/humidity", HTTP_GET, [](AsyncWebServerRequest * request) {
    request->send_P(200, "text/plain", Humidity.c_str());
    delay(20);
  });

  server.on("/timer", HTTP_GET, [](AsyncWebServerRequest * request) {
    String inputMessage;
    String inputParam;
    String message;

    if (request->hasParam(PARAM_INPUT_IRRI_TIMER)) {
      inputMessage = request->getParam(PARAM_INPUT_IRRI_TIMER)->value();
      inputParam = PARAM_INPUT_1;
      Serial.println(inputMessage);
      destination = 0xAC;
      message = inputMessage;
      sendMessage(message);
      delay(100);
    }
    else {
      inputMessage = "No message sent";
      inputParam = "none";
    }

    request->send(200, "text/html", "Timer for irrigation set to: "
                  + inputMessage +
                  "<br><a href=\"/\">Return to Home Page</a>");
    delay(20);
  }); 
  server.begin();
}

void loop() {
  onReceive(LoRa.parsePacket());
}

void sendMessage(String outgoing) {
  LoRa.beginPacket();                   // start packet
  LoRa.write(destination);              // add destination address
  LoRa.write(localAddress);             // add sender address
  LoRa.write(msgCount);                 // add message ID
  LoRa.write(outgoing.length());        // add payload length
  LoRa.print(outgoing);                 // add payload
  LoRa.endPacket();                     // finish packet and send it
  msgCount++;                           // increment message ID
}

void onReceive(int packetSize) {
  delay(20);
  //  Serial.println("Receive");
  if (packetSize == 0) return;          // if there's no packet, return

  // read packet header bytes:
  int recipient = LoRa.read();          // recipient address
  byte sender = LoRa.read();            // sender address
  byte incomingMsgId = LoRa.read();     // incoming msg ID
  byte incomingLength = LoRa.read();    // incoming msg length

  String incoming = "";

  while (LoRa.available()) {
    incoming += (char)LoRa.read();
  }

  if (incomingLength != incoming.length()) {   // check length for error
    Serial.println("error: message length does not match length");
    return;                             // skip rest of function
  }

  // if the recipient isn't this device or broadcast,
  if (recipient != localAddress && recipient != 0xFF) {
    Serial.println("This message is not for me.");
    return;                             // skip rest of function
  }

  if (incoming.indexOf("#t") != -1){
      incoming.remove(5, 2);
      Temperature = String(incoming);
  }
  else if (incoming.indexOf("#h") != -1){
      incoming.remove(5, 2);
      Humidity = String(incoming);
  }
    
  // if message is for this device, or broadcast, print details:
  Serial.println("Received from: 0x" + String(sender, HEX));
  Serial.println("Sent to: 0x" + String(recipient, HEX));
  Serial.println("Message ID: " + String(incomingMsgId));
  Serial.println("Message length: " + String(incomingLength));
  Serial.println("Message: " + incoming);
  Serial.println("RSSI: " + String(LoRa.packetRssi()));
  Serial.println("Snr: " + String(LoRa.packetSnr()));
  Serial.println();
}
