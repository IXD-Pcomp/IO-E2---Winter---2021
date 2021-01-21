// Adafruit IO Digital Output Example
// Tutorial Link: https://learn.adafruit.com/adafruit-io-basics-digital-output
//
// Adafruit invests time and resources providing this open source code.
// Please support Adafruit and open source hardware by purchasing
// products from Adafruit!
//
// Written by Todd Treece for Adafruit Industries
// Copyright (c) 2016 Adafruit Industries
// Licensed under the MIT license.
//
// All text above must be included in any redistribution.

/************************** Configuration ***********************************/

// edit the config.h tab and enter your Adafruit IO credentials
// and any additional configuration needed for WiFi, cellular,
// or ethernet clients.
#include "config.h"

/************************ Example Starts Here *******************************/

// digital pin, pin that goes to your LED
#define LED_PIN 13

// set up the 'digital' feed
//put in your feed name you created in Adafruit IO
AdafruitIO_Feed *testData = io.feed("testData"); //New!


void setup() {
  
  pinMode(LED_PIN, OUTPUT);
  
  // start the serial connection
  Serial.begin(115200);

  // wait for serial monitor to open
  while(! Serial);

  // connect to io.adafruit.com
  Serial.print("Connecting to Adafruit IO");
  io.connect();

  // set up a message handler for the 'digital' feed.
  // the handleMessage function (defined below)
  // will be called whenever a message is
  // received from adafruit io.

  //change to your feed name you created in Adafruit IO
  testData->onMessage(handleMessage); //New!

  // wait for a connection
  while(io.status() < AIO_CONNECTED) {
    Serial.print(".");
    delay(500);
  }

  // we are connected
  Serial.println();
  Serial.println(io.statusText());

  //change to your feed name you created in Adafruit IO
  testData->get(); //New!

}

void loop() {

  // io.run(); is required for all sketches.
  // it should always be present at the top of your loop
  // function. it keeps the client connected to
  // io.adafruit.com, and processes any incoming data.
  io.run();

}

// this function is called whenever an 'digital' feed message
// is received from Adafruit IO. it was attached to
// the 'digital' feed in the setup() function above.

//change to your feed name you created in Adafruit IO
void handleMessage(AdafruitIO_Data *testData) { // New!

  Serial.print("received <- ");

//change to your feed name you created in Adafruit IO
  if(testData->toPinLevel() == HIGH) // New!
    Serial.println("HIGH");
  else
    Serial.println("LOW");

//change to your feed name you created in Adafruit IO
  digitalWrite(LED_PIN, testData->toPinLevel()); // New!
}
