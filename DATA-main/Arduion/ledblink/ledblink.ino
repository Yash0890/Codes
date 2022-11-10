//#define BUILTIN_LED 5
int LED = D5;
int LED1 = D4;
void setup() {
  pinMode(LED, OUTPUT);     // Initialize the BUILTIN_LED pin as an output
  pinMode(LED1, OUTPUT);     // Initialize the BUILTIN_LED pin as an output
digitalWrite(LED, LOW);
digitalWrite(LED1, LOW);
}

// the loop function runs over and over again forever
void loop() {
  digitalWrite(LED, HIGH); // Turn the LED on (Note that LOW is the voltage level
  delay(1000);
  digitalWrite(LED, LOW);                                  // but actually the LED is on; this is because 
                                    // it is acive low on the ESP-01)
                      // Wait for a second
  digitalWrite(LED1, HIGH);  // Turn the LED off by making the voltage HIGH
  delay(1000);
  digitalWrite(LED1, LOW);
  delay(1000);                      // Wait for two seconds (to demonstrate the active low LED)
}

