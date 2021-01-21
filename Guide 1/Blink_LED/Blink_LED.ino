
#define LED_PIN 13

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(LED_PIN, OUTPUT);
}

// the loop function runs over and over again forever
void loop() {

  digitalWrite(LED_BUILTIN, LOW);
  // Turn the LED on (Note that LOW is the voltage level
  // but actually the LED is on; this is because
  // it is active low on the ESP-01)
  digitalWrite(LED_PIN, HIGH);
  // The Builtin LED isnt active so it is set the opposite


  delay(1000);// Wait for a second
  digitalWrite(LED_PIN, LOW); // Turn the LED off by making the voltage HIGH
  digitalWrite(LED_BUILTIN, HIGH);
  delay(2000);// Wait for two seconds (to demonstrate the active low LED)
}
