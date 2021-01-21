
#define BUTTON_PIN 16
#define LED_PIN 13 


void setup() {
  pinMode(BUTTON_PIN, INPUT);// Initialize the BUTTON_PIN as an input
  pinMode(LED_PIN, OUTPUT); // Initialize the LED_PIN as an output

  
}

// the loop function runs over and over again forever
void loop() {
  
  if(digitalRead(BUTTON_PIN) == HIGH){
    
    digitalWrite(LED_PIN, HIGH);
    
  }else{
    
    digitalWrite(LED_PIN, LOW);
  }
}
  
