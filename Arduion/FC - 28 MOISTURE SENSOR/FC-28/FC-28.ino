/*
 * FC - 28  
 * MOISTURE SENSOR
 * it gives both analog and digital output
 * analog output range 0 to 100 %
 * dry soil 550
 * wet soil 10
 * 
*/
int  s_pin = A0;                        // sensor pin (input)
int output_value;

void setup(){
  Serial.begin(9600);                  // connecting serial moniter to arduino 
  Serial.println("Getting Reading");    
  delay(2000);
}

void loop(){
  output_value = analogRead(s_pin);                // Analog READING
  output_value = digitalRead(s_pin);               // DIGITAL READING
  output_value=map(output_value,550,10,100,0);     // comparing the output value between limit of (wet and dry value) and percantage (0 to 100))
  Serial.print("Moisture Value");
  Serial.print(output_value);
  Serial.print("%");
  Serial.println(F("---------------------------------------------")); // printing closing line
  delay(1000);
}
 
