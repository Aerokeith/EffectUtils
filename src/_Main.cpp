/*
#include <Arduino.h>
#include "Randomizer.h"

randomizerClass randPix;

const uint16_t numPixels = 10;
const uint8_t numTypes = 10;

void setup() {
  Serial.begin(115200);
  randomSeed(4013687787);
  randPix.init(numPixels, numTypes, 0.01);
  delay(3000);
}


void loop() {
  for (uint16_t p = 0; p < numPixels; p++) {
    Serial.printf("%hu ", randPix.getPixType(p));
  }
  Serial.println();
  randPix.randomize();
  delay(1000);
}

*/