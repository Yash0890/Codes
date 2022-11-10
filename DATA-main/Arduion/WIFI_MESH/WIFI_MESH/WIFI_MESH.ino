#include "painlessMesh.h"

#define MESH_PREFIX   "WIFIMESH"
#define MESH_PASSWORD "1234567890"
#define MESH_PORT     5555

Scheduler userScheduler;
painlessMesh mesh;

void sendMessage() ;

Task taskSendMessage(TASK_SECOND*1,TASK_FOREVER,&sendMessage);

void sendMessage(){
  String msg = "Hello from Node 4 ";
  msg += mesh.getNodeId();
  mesh.sendBroadcast(msg);
  taskSendMessage.setInterval(random(TASK_SECOND*1,TASK_SECOND*5));
}

void receivedCallback(uint32_t from,String &msg){
   Serial.printf("startHere: Received from %u msg=%s\n", from, msg.c_str());
}

void newConnectionCallback(uint32_t nodeId){
  Serial.printf("--> starthere : New connection, nodeId = %u\n",nodeId);
}

void changedConnectionCallback(){
  Serial.printf("Changed connections\n");
}

void nodeTimeAdjustedCallBack(uint32_t offset){
  Serial.printf("Adjusted Time %u. Offset = %d\n",mesh.getNodeTime(),offset);
}

void setup(){
  Serial.begin(115200);
  mesh.setDebugMsgTypes(ERROR | STARTUP);
  mesh.init (MESH_PREFIX,MESH_PASSWORD,&userScheduler,MESH_PORT);
  mesh.onReceive(&receivedCallback);
  mesh.onNewConnection(&newConnectionCallback);
  mesh.onChangedConnections(&changedConnectionCallback);

  userScheduler.addTask(taskSendMessage);
  taskSendMessage.enable();
}

void loop() {
  // it will run the user scheduler as well
  mesh.update();
}
