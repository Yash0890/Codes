#include <ESP8266WiFi.h>

const char* Wifi_ssid = "1234" ;
const char* Wifi_pass = "Yash@Test";

WiFiServer server(80);
 
String header = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n";
String html_1 = "<!DOCTYPE html><html><head><title>MOTOR Control</title></head><body><div id='main'><h2>MOTOR Control</h2>";
String html_2 = "<form id='F1' action='CLOCKWISE'><input class='button' type='submit' value='CLOCKWISE' ></form><br>";
String html_3 = "<form id='F2' action='ANTICLOCKWISE'><input class='button' type='submit' value='ANTI CLOCKWISE' ></form><br>";
String html_4 = "<form id='F3' action='STOP'><input class='button' type='submit' value='STOP' ></form><br>";
String html_5 = "</div></body></html>";

 
String request = "";
int MOTOR_Pin_1 = D1;
int MOTOR_Pin_2 = D2;
int value = 0;
void setup() 
{
    pinMode(MOTOR_Pin_1, OUTPUT); 
    pinMode(MOTOR_Pin_2, OUTPUT); 
    Serial.begin(115200);
    WiFi.begin(Wifi_ssid, Wifi_pass);
    server.begin();
    while (WiFi.status() != WL_CONNECTED) 
    {
      delay(500); 
      Serial.print(".");
    }
    Serial.println(WiFi.localIP());
} // void setup()
 
 
void loop() 
{
 
    // Check if a client has connected
    WiFiClient client = server.available();
    if (!client)  {  return;  }
 
    // Read the first line of the request
    request = client.readStringUntil('\r');
 
    if       ( request.indexOf("ANTICLOCKWISE") > 0 )  
    { 
      digitalWrite(MOTOR_Pin_1, HIGH);
      digitalWrite(MOTOR_Pin_2, LOW);  
      Serial.println("ANTI-CLOCKWISE");
      value = 1;
    }
    else if  ( request.indexOf("CLOCKWISE") > 0 ) 
    { 
      digitalWrite(MOTOR_Pin_1, LOW);
      digitalWrite(MOTOR_Pin_2, HIGH);
      Serial.println("Clockwise");
      value =2;
    }
    else if  ( request.indexOf("STOP") > 0 ) 
    { 
      digitalWrite(MOTOR_Pin_1, LOW);
      digitalWrite(MOTOR_Pin_2, LOW);
      Serial.println("stop");
      value =3;
    }

    client.flush();
 
    client.print( header );
    client.print( html_1 );
    client.print( html_2 );
    client.print( html_3 );
    client.print( html_4 );
    client.print("MOTOR STATUS :");
    if(value == 1) {client.print("ANTI-CLOCKWISE");}
    else if(value == 2){client.print("CLOCLWISE");}
    else{client.print("STOP");}
    client.print( html_5 );
    
    delay(5);
  // The client will actually be disconnected when the function returns and 'client' object is detroyed
 
}
