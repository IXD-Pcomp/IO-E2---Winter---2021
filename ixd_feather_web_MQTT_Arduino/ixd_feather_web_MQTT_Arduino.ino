///////////////////////////////////////////////////////////////
//////////Written By Steve Hudak using////////////////////////
//////////Adafruit MQTT and others///////////////////////////
////////////////////////////////////////////////////////////


// edit the config.h tab and enter your Adafruit IO credentials
// and any additional configuration needed for WiFi, cellular,
// or ethernet clients.
#include "config.h"
#include <ESP8266WiFi.h>
#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"

//// Defines one analog input button pin on A0
#define Buttons A0

////
int ButtonRead = 0;
int current = 0;
int value = 0;
int last = -1;

#define LED_PIN1 13
#define LED_PIN2 12

/************ Global State (you don't need to change this!) ******************/

// Create an ESP8266 WiFiClient class to connect to the MQTT server.
WiFiClient client;

// Sets up the MQTT client class by passing in the WiFi client and MQTT server and login details.
Adafruit_MQTT_Client mqtt(&client, AIO_SERVER, AIO_SERVERPORT, AIO_USERNAME, AIO_KEY);

/****************************** Feeds ***************************************/
///// THIS PROCESS REQUIRES 2 ADAFRUIT IO FEEDS, ONE FOR FEATHER DATA AND ONE FOR P5 DATA

///// Publish uses a variable named featherButtons to hold the current value from your feather
//// to be SENT TO the Adafruit IO feed, replace 'yourFeed' with the name of this Adafruit IO feed
Adafruit_MQTT_Publish featherButtons = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/Feed1");

//// Subscribe uses a variable named p5Buttons to hold the current value it is receiving FROM THE Adafruit
//// IO feed data your web P5 sketch is sending to the Adafruit server replace 'yourFeed' with the name
//// of your 2nd Adafruit IO feed
Adafruit_MQTT_Subscribe p5Buttons = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME "/feeds/Feed2");

/*************************** Sketch Code ************************************/

//// calls the MQTT function to connect to the Adafruit IO server
void MQTT_connect();

void setup() {

  //// sets your Led pins, the cuircuit for this code uses 2 component leds and the built-in Led
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(LED_PIN1, OUTPUT);
  pinMode(LED_PIN2, OUTPUT);

  //// starts the serial connection
  Serial.begin(115200);
  Serial.print("Connecting to ");
  Serial.println(WIFI_SSID);
  WiFi.begin(WIFI_SSID, WIFI_PASS);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println();
  Serial.println("WiFi connected");
  Serial.println("IP address: "); Serial.println(WiFi.localIP());

  //// Sets up MQTT subscription for p5Buttons feed.
  mqtt.subscribe(&p5Buttons);
}

void loop() {

  // Ensure the connection to the MQTT server is alive (this will make the first
  // connection and automatically reconnect when disconnected).  See the MQTT_connect
  // function definition further below.
  MQTT_connect();

//// BEGIN ---- USES DATA COMING IN FROM THE P5 SKETCH

  //// This uses the data from the p5Buttons feed and looks for specific characters
  Adafruit_MQTT_Subscribe *subscription;
  while ((subscription = mqtt.readSubscription(0))) {
    if (subscription == &p5Buttons) {
      Serial.print(F("IN: "));
      Serial.println((char *)p5Buttons.lastread);
      
      //// this if-statement tells the LED_PIN1 to turn on if the data has a "1"
      if (strcmp((char *)p5Buttons.lastread, "1") == 0) {
        digitalWrite(LED_PIN1, HIGH);
        delay(300);
        //// and turn off LED_PIN1 if the data does not have a "1"
      } else {
        digitalWrite(LED_PIN1, LOW);
      }
      //// this if-statement tells the LED_PIN2 to turn on if the data has a "2"
      if (strcmp((char *)p5Buttons.lastread, "2") == 0) {
        digitalWrite(LED_PIN2, HIGH);
        delay(300);
        //// and turn off LED_PIN2 if the data does not have a "2"
      } else {
        digitalWrite(LED_PIN2, LOW);
      }
    }
  }
  /// END ---- USES DATA COMING IN FROM THE P5 SKETCH

//// BEGIN ---- SENDS DATA GOING OUT FROM THE FEATHER

  //// The analog input runs through a series of resistors isolating each button 
  //// in series after a different resistance button so each button press reads 
  //// a different analog voltage output
  
  //// reads the value from A0 every loop
  ButtonRead = analogRead(Buttons);
Serial.print(A0  );
Serial.println(analogRead(A0));

  ////////THE CURRENT RESISTANCE FROM THE RESISTOR-BUTTON-CIRCUIT
  ///////can add as many as required but new ones must be re-measured
  if (ButtonRead > 800 && ButtonRead < 900) {
    //// ASSIGNS THE current RANGE TO '1' HELD IN THE current VARIABLE
    current = 1;
    Serial.print(F("OUT: "));
    Serial.println(current);
    //// the LEDS also turn on and off to confirm BUTTON PRESSES
    digitalWrite(LED_PIN2, HIGH);
    digitalWrite(LED_BUILTIN, HIGH);
  }
  else if (ButtonRead > 1000) {
    //// ASSIGNS THE current RANGE TO '2' HELD IN THE current VARIABLE
    current = 2;
    Serial.print(F("OUT: "));
    Serial.println(current);
    //// the LEDS also turn on and off to confirm BUTTON PRESSES
    digitalWrite(LED_PIN1, HIGH);
    digitalWrite(LED_BUILTIN, HIGH);

  } else {
    //// ASSIGNS THE current RANGE TO A SPECIFIC OUTPUT NUMBER HELD IN THE current VARIABLE
    current = 0;
    Serial.print(F("OUT: "));
    Serial.println(current);
    //// the LEDS also turn on and off to confirm BUTTON PRESSES
    digitalWrite(LED_PIN1, LOW);
    digitalWrite(LED_PIN2, LOW);
    digitalWrite(LED_BUILTIN, LOW);
  }

  //// Checks if the current value has changed state
  if (current == last)
    return;
    //// defines value at the most current state 
  int32_t value = current;

  //// END ---- SENDS DATA GOING OUT FROM THE FEATHER

//// this if-statement publishes the final 'value' variable holding the data
  if (! featherButtons.publish(value++)) {
    Serial.println(F("Failed"));
  } else {
    Serial.println(value);
    //////////DO NOT REDUCE BELOW 2500
    //////////THIS MAINTAINS THE OUTPUT at every 2.5 seconds 
    /////////AT MAXIMUM ADAFRUIT IO THRESHOLD
    delay(2500);
  }
} //// END of void loop

//// you shouldn't change anything below this line
//// Function to connect and reconnect as necessary to the MQTT server.
//// Should be called in the loop function and it will take care if connecting.
void MQTT_connect() {
  int8_t ret;

  // Stop if already connected.
  if (mqtt.connected()) {
    return;
  }

  Serial.print("Connecting to MQTT... ");

  uint8_t retries = 3;
  while ((ret = mqtt.connect()) != 0) { // connect will return 0 for connected
    Serial.println(mqtt.connectErrorString(ret));
    Serial.println("Retrying MQTT connection in 5 seconds...");
    mqtt.disconnect();
    delay(5000);  // wait 2 seconds
    retries--;
    if (retries == 0) {
      // basically die and wait for WDT to reset me
      while (1);
    }
  }
  Serial.println("MQTT Connected!");
}
