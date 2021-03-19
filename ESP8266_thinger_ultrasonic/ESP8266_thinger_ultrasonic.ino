#include <ThingerESP8266.h>
#include <ESP8266WiFi.h>
#include <Servo.h>

#define USERNAME "your_username_here"
#define DEVICE_ID "your_id_here"
#define DEVICE_CREDENTIAL "your_device_credential_here"

#define SSID "you_wifi_id_here"
#define SSID_PASSWORD "your_wifi_password_here"

ThingerESP8266 thing(USERNAME, DEVICE_ID, DEVICE_CREDENTIAL);


// utlrasonic pinout
#define TRIGGER_PIN  14 // D5
#define ECHO_PIN     12 //D6 


void setup() {
  Serial.begin(115200);

  thing.add_wifi(SSID, SSID_PASSWORD);


   pinMode(TRIGGER_PIN, OUTPUT);
 pinMode(ECHO_PIN, INPUT);

  thing["SONIC"] >> [] (pson& out){
      double duration, distance;
      digitalWrite(TRIGGER_PIN, LOW);  // Get Start
      delayMicroseconds(2); // stable the line 
      digitalWrite(TRIGGER_PIN, HIGH); // sending 10 us pulse
      delayMicroseconds(10); // delay 
      digitalWrite(TRIGGER_PIN, LOW); // after sending pulse wating to receive signals 
      duration = pulseIn(ECHO_PIN, HIGH); // calculating time 
      distance = (duration/2) / 29.1; // single path 
      //Serial.println(distance);
      out = distance;
      
  };
  ////END OF SETUP
}

//// Thinger handles everything in the loop

void loop() {
  //Serialprintln("test");
  thing.handle();

}
