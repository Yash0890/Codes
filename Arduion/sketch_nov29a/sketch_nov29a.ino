
#define AOUT_PIN3 34

void setup() {
Serial.begin(115200);
}

void loop() {
// Plant 1
// int value1 = analogRead(AOUT_PIN1); 

// // Plant 2
// int value2 = analogRead(AOUT_PIN2); 

//Plant 3
int value3 = analogRead(AOUT_PIN3); 

char val[100];
sprintf(val,"\n\n\n\nMoisture value \nSensor 1: \n\nSensor 2: \n\nSensor 3: %d",value3);
Serial.println(val);
delay(2000);
}