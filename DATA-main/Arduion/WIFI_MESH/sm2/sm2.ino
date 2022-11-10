// salve
#include <ESP8266WiFi.h>
#include <espnow.h>

#define led D1

uint8_t mastermac[] = {0xB4, 0xE6, 0X2D, 0x34, 0xA7, 0x56};

typedef struct struct_message {
    int a; // input
} struct_message;

typedef struct struct_message_1 {
    int b; // output
} struct_message_1;

struct_message Data_1;
struct_message_1 Data_2;

unsigned long lastTime = 0;
unsigned long timerDelay = 2000;

void OnDataSent(uint8_t *mac_addr,uint8_t sendStatus){
  Serial.print("Last Packet Send Stauts : ");
  if(sendStatus == 0){
    Serial.println("Delivery Done");
  }
  else{
    Serial.println("Delivery Fali");
  }
}

void OnDataRecv(uint8_t *mac, uint8_t *incomingData, uint8_t  len){
  memcpy(&Data_1, incomingData, sizeof(Data_1));
  Serial.print("Out : ");
  Serial.println(Data_2.b);
  Serial.print("In : ");
  Serial.println(Data_1.a);
  Serial.println();
}

void setup(){
  Serial.begin(115200);
  pinMode(led,OUTPUT);
  WiFi.mode(WIFI_STA);
  if(esp_now_init()!=0){
    Serial.println("Error Initializing ESP_NOW");
    return;
  }
  esp_now_set_self_role(ESP_NOW_ROLE_CONTROLLER);
  esp_now_register_send_cb(OnDataSent);
  esp_now_set_self_role(ESP_NOW_ROLE_SLAVE);
  esp_now_register_recv_cb(OnDataRecv);
  esp_now_add_peer(mastermac,ESP_NOW_ROLE_SLAVE,1,NULL,0);
}

void loop(){
  if((millis()-lastTime)>timerDelay){
    
    
    if((Data_1.a == "SP")==true){
      digitalWrite(led,HIGH);
      Data_2.b = "1";
    }
    else{
      digitalWrite(led,LOW);
      Data_2.b = "0";
    }
    
    esp_now_send(mastermac,(uint8_t *) &Data_2, sizeof(Data_2));

    lastTime = millis();
  }
}
