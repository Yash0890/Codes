int soil=A0;
int relay=3;

void setup() {

pinMode(soil,INPUT);
pinMode(relay,OUTPUT);
digitalWrite(relay,HIGH);
Serial.begin(9600);
}

void loop() {

int soilData=analogRead(soil);
Serial.print("Soil DATA:");
Serial.println(soilData);

if(soilData>900)
{
  digitalWrite(relay,LOW);
}
else
{
  digitalWrite(relay,HIGH);
}
}
