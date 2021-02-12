/*
 Controlling a servo position using a potentiometer (variable resistor)
 by Michal Rinott <http://people.interaction-ivrea.it/m.rinott>

 modified on 8 Nov 2013
 by Scott Fitzgerald
 http://www.arduino.cc/en/Tutorial/Knob
*/

/* Modified Feb 2021 Doug Whitton
 * Use a potentiometer to send data to adafruit.io
 * 
 */

#include "config.h"

//this library is optional, it depends on the type of sensor being used
//if a type of sensor requires a specific library, add it here
//#include "Adafruit_Sensor.h"

// digital pin, pin that goes to your LED
//#define LED_PIN 13
//#define BUTTON_PIN 16
// set up the 'digital' feed or in this case
//put in your feed name you created in Adafruit IO
AdafruitIO_Feed *testData = io.feed("testData"); //New!



int potpin = A0;  // analog pin used to connect the potentiometer
int val;    // variable to read the value from the analog pin

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
 
  
  
  Serial.begin(115200);
  //myservo.attach(9);  // attaches the servo on pin 9 to the servo object
}

void loop() {
  io.run();
  val = analogRead(potpin);            // reads the value of the potentiometer (value between 0 and 1023)
  val = map(val, 0, 1023, 0, 100);     // scale it to use it with your application, in this example, it's a min/max range of 100
  
  //delay(30000); // waits for the servo to get there - 30 second delay
  delay(30);// 30 micro second delay
  Serial.println(val);
  
//place your specific feed name

  testData->save(val); //New! - "testData" needs to be re-named based on the name of the feed on Adafruit.io  
}
