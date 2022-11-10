//MASTER

#include <ESP8266WiFi.h>
#include <espnow.h>

// REPLACE WITH RECEIVER MAC Address
uint8_t slavemac[] = {0xE8, 0xDB, 0X84, 0xAE, 0x22, 0x40};

// Structure example to send data
// Must match the receiver structure
typedef struct struct_message 
{
  String a;
} 
struct_message;

typedef struct struct_message1
{
    String b;
}struct_message1;

// Create a struct_message called myData
struct_message masData;
struct_message1 slaveData;


unsigned long lastTime = 0;  
unsigned long timerDelay = 2000;  // send readings timer

// Callback when data is sent
void OnDataSent(uint8_t *mac_addr, uint8_t sendStatus) {
  Serial.print("Last Packet Send Status: ");
  if (sendStatus == 0){
    Serial.println("Delivery success");
  }
  else{
    Serial.println("Delivery fail");
  }
} 
 // Callback when data is recieve
void OnDataRecv(uint8_t * mac, uint8_t *incomingData, uint8_t len) {
  memcpy(&slaveData, incomingData, sizeof(slaveData));
  Serial.print("String: ");
  Serial.println(slaveData.b);
  Serial.println();
}
 



 
void setup() {
  // Init Serial Monitor
  Serial.begin(115200);
 
  // Set device as a Wi-Fi Station
  WiFi.mode(WIFI_STA);

  // Init ESP-NOW
  if (esp_now_init() != 0) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  // Once ESPNow is successfully Init, we will register for Send CB to
  // get the status of Trasnmitted packet
  esp_now_set_self_role(ESP_NOW_ROLE_CONTROLLER);
  esp_now_register_send_cb(OnDataSent);
  esp_now_set_self_role(ESP_NOW_ROLE_SLAVE);
  esp_now_register_recv_cb(OnDataRecv);
  
  // Register peer
  esp_now_add_peer(slavemac
  , ESP_NOW_ROLE_SLAVE, 1, NULL, 0);
}
 
void loop() {
  if ((millis() - lastTime) > timerDelay) {
    // Set values to send
    //strcpy(myData.a, "THIS IS A CHAR");
    masData.a = "Hi Slave";

    // Send message via ESP-NOW
    esp_now_send(slavemac, (uint8_t *) &masData, sizeof(masData));

    lastTime = millis();
  }
}
