#include <ESP8266WiFi.h>
 
const char* ssid = "1234";
const char* password = "Yash@Test";
 
int LED = D1;
int INR_LED = 16;// led connected to D0
WiFiServer server(80);
 
void setup() 
{
  Serial.begin(115200);
  pinMode(LED, OUTPUT);
  pinMode(INR_LED,OUTPUT);
  digitalWrite(LED, LOW);
  
  
  Serial.print("Connecting to Internet ");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) 
  {
    digitalWrite(INR_LED, LOW);   // turn the LED on (HIGH is the voltage level)
    delay(500);                       // wait for a second
    digitalWrite(INR_LED, HIGH);    // turn the LED off by making the voltage LOW
    delay(500); 
    Serial.print(".");
  }
  
  Serial.println("WiFi connected");
 
 /*-------- server started---------*/ 
  server.begin();
  Serial.println("Server started");
 
  /*------printing ip address--------*/
  Serial.print("IP Address of network: ");
  Serial.println(WiFi.localIP());
  Serial.print("Copy and paste the following URL: http://");
  Serial.print(WiFi.localIP());
  Serial.println("/");
}
 
void loop() 
  {
    
    WiFiClient client = server.available();    
    if (!client) 
    {
      return;
    }
  Serial.println("Waiting for new client");   
  while(!client.available())
  {
    delay(1);
  }
 
  String request = client.readStringUntil('\r');
  Serial.println(request);
  client.flush(); // wait till buffering end
 
 
  int value = LOW;
  if (request.indexOf("/LED=ON") != -1)  
  {
    digitalWrite(LED, HIGH);
    value = HIGH;
  }
  if (request.indexOf("/LED=OFF") != -1)  
  {
    digitalWrite(LED, LOW);
    value = LOW;
  }
 
/*------------------Creating html page---------------------*/

  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: text/html");
  client.println(""); 
  client.println("<!DOCTYPE HTML>");
  client.println("<html>"); // starting
 
  client.print("LED is: ");
 
  if(value == HIGH) 
  {
    client.print("ON");
  } 
  else 
  {
    client.print("OFF");
  }
  client.println("<br><br>");
  client.println("<a href=\"/LED=ON\"\"><button>ON</button></a>");
  client.println("<a href=\"/LED=OFF\"\"><button>OFF</button></a><br />");  
  client.println("</html>"); // ending
 
  delay(1);
  Serial.println("Client disonnected");
  Serial.println("");
 
}
