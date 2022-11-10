//SLAVE

#include <ESP8266WiFi.h>
#include <espnow.h>

#define led D3


uint8_t mastermac[] = {0xB4, 0xE6, 0X2D, 0x34, 0xA7, 0x56};
// Structure example to receive data
// Must match the sender structure
 typedef struct struct_message {
    String a;
} struct_message;

 typedef struct struct_message1
 {
   String b;
 } struct_message1;

// Create a struct_message called myData
struct_message masData;
struct_message1 slaveData;

unsigned long lastTime = 0;  
unsigned long timerDelay = 2000; 

void OnDataSent(uint8_t *mac_addr, uint8_t sendStatus) {
  Serial.print("Last Packet Send Status: ");
  if (sendStatus == 0){
    Serial.println("Delivery success");
  }
  else{
    Serial.println("Delivery fail");
  }
} 

// Callback function that will be executed when data is received
void OnDataRecv(uint8_t * mac, uint8_t *incomingData, uint8_t len) {
  memcpy(&masData, incomingData, sizeof(masData));
  Serial.print("String: ");
  Serial.println(masData.a);
  Serial.println();
 
}
 
void setup() {
  // Initialize Serial Monitor
  Serial.begin(115200);
  pinMode(led, OUTPUT);
  
  // Set device as a Wi-Fi Station
  WiFi.mode(WIFI_STA);

  // Init ESP-NOW
  if (esp_now_init() != 0) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }
  
  // Once ESPNow is successfully Init, we will register for recv CB to
  // get recv packer info
  esp_now_set_self_role(ESP_NOW_ROLE_SLAVE);
  esp_now_register_recv_cb(OnDataRecv);
  esp_now_set_self_role(ESP_NOW_ROLE_CONTROLLER);
  esp_now_register_send_cb(OnDataSent);
}

void loop() {
    if ((millis() - lastTime) > timerDelay) {
      if((masData.a== "Hi Slave")==true)
      {
        digitalWrite(led, HIGH);
        Serial.println("LED turned on");
      }
      else
      {
         digitalWrite(led, LOW);
      }

    // Send message via ESP-NOW
    esp_now_send(mastermac, (uint8_t *) &slaveData, sizeof(slaveData));

    lastTime = millis();
  }
}
