#include <Arduino.h>
#include <Main/config.h>
#include <Adafruit_NeoPixel.h>

// How many internal neopixels do we have? some boards have more than one!
#define NUMPIXELS        5

Adafruit_NeoPixel pixels(NUMPIXELS, LED_STRIP, NEO_GRB + NEO_KHZ800);

// the setup routine runs once when you press reset:
void setup() {
  Serial.begin(9600);
  pixels.begin(); // INITIALIZE NeoPixel strip object (REQUIRED)
  pixels.setBrightness(100); // not so bright
}

// the loop routine runs over and over again forever:
void loop() {
  // say hi
  Serial.println("Hello!");
  
  // set color to red
  pixels.fill(0xFF0000);
  pixels.show();
  delay(500); // wait half a second

  // turn off
  pixels.fill(0x000000);
  pixels.show();
  delay(500); // wait half a second
}