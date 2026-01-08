//
// OP-Z Cube
// (c) matesteinforth.com 2020
//
// based on DMXUSB https://github.com/DaAwesomeP/dmxusb/
// uses FastLED https://github.com/FastLED/FastLED

//
#define FASTLED_ALLOW_INTERRUPTS 0
#include "DMXUSB.h"
#include <FastLED.h>

#define DATA_PIN 3 // data pin
#define BRIGHTNESS 164 // LED brightness
#define MAXLED 1 // maximum LEDs per channel

//
// Define Matrix Mapping ala https://forum.arduino.cc/index.php?topic=45681.0
const byte map_array[16][MAXLED] PROGMEM = {
  { 1},
  { 2},
  { 3},
  { 4},
  { 5},
  { 6},
  { 7},
  { 8},
  { 9},
  { 10},
  { 11},
  { 12},
  { 13},
  { 14},
  { 15},
  { 16}
};


//
// no user serviceable parts down here
#define DMXUSB_BAUDRATE 115200 // Recommended Arduino baud rate: 115200 - DMX baud rate: 250000
#define NUM_LEDS 16*MAXLED+10 // Number of LEDs

// Define the array of LEDs
CRGB leds[NUM_LEDS];

// receive a DMX transmission, write data to LEDs and show it
void showLEDs(int universe, char buffer[512]) {

  // iterating over the channels
  for (int index = 0; index < 16; index++) {
    for ( int j = 0; j < MAXLED; ++j ) {
      byte myBarMain = pgm_read_byte(&(map_array[index][j])); // look up the LED ID in the mapping table
      if ( myBarMain > 0) { // all elements with 0 are passed, LED IDs are shifted by 1
        leds[myBarMain - 1] = CRGB(buffer[index * 3 + 1], buffer[index * 3], buffer[index * 3 + 2]); // GRB order
      }
    }
  }
  FastLED.show(); // Display the frame after processing all channels
}

DMXUSB DMXPC(
  // Stream serial,
  Serial,
  // int baudrate,
  DMXUSB_BAUDRATE,
  // int mode,
  // With mode==1, the library processes two universes for a total of 1024 DMX channels
  0,
  // void (*dmxInCallback)(int universe, unsigned int index, char buffer[512])
  showLEDs
);

void setup() {
  Serial.begin(DMXUSB_BAUDRATE);
  FastLED.addLeds<WS2812B, DATA_PIN, RGB>(leds, NUM_LEDS);
  FastLED.setBrightness(BRIGHTNESS);
}

void loop() {
  DMXPC.listen();
}
