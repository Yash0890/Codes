// master
#include <ESP8266WiFi.h>
#include <espnow.h>

#define Switch_pin D2
cont int led_1 D1

uint8_t slavemac[] = {0xE8, 0xDB, 0X84, 0xAE, 0x22, 0x40};

typedef struct struct_message {
  int a; // output
} struct_message;

typedef struct struct_message_1 {
  int b; // input
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
  memcpy(&Data_2, incomingData, sizeof(Data_2));
  Serial.print("Out : ");
  Serial.println(Data_1.a);
  Serial.print("In : ");
  Serial.println(Data_2.b);
  Serial.println();
}

void setup(){
  Serial.begin(115200);
  pinMode(Switch_pin,INPUT);
  pinMode(led_1,OUTPUT);
  WiFi.mode(WIFI_STA);
  if(esp_now_init()!=0){
    Serial.println("Error Initializing ESP_NOW");
    return;
  }
  esp_now_set_self_role(ESP_NOW_ROLE_CONTROLLER);
  esp_now_register_send_cb(OnDataSent);
  esp_now_set_self_role(ESP_NOW_ROLE_SLAVE);
  esp_now_register_recv_cb(OnDataRecv);
  esp_now_add_peer(slavemac,ESP_NOW_ROLE_SLAVE,1,NULL,0);
}

void loop(){
  if((millis()-lastTime)>timerDelay){
    if(Switch_pin == HIGH){
      digitalWrite(led_1,HIGH);
      Data_1.a = "SP";
    }
    else{
      digitalWrite(led_1,LOW);
      Data_1.a = "SR";
    }
    
    if((Data_2.b== "1")==true){
      Serial.println("LED ON");
    }
    else{
      Serial.println("LED OFF");
    }
    esp_now_send(slavemac,(uint8_t *) &Data_1, sizeof(Data_1));

    lastTime = millis();
  }
}
