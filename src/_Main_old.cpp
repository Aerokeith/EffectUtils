/*

#include <Arduino.h>
#include <elapsedMillis.h>
#include "Sine.h"
// #include "StripCoord.h"
#include "StripMgr.h"

stripMgrClass strip;

const segmentDefStruct segmentDef[2] = {
  { {0, 0},   0,  3 },
  { {10, 10}, 45, 4 } 
};

// sineClass mySine;

// stripCoordClass myStrip[2];

// float framePeriod = 100;
// int count = 0;
// elapsedMillis frameTimer;   // timer for recurring execution of frame-based functions

// const stripDefStruct stripDef[2] = {
//   { {0, 0}, 0, 1 },
//   { {10, 10}, 0, 1}
// };

void setup() {

  // float pos;
  stripCoordStruct coord;

  delay(10000);
  strip.define(7, 2, 1, segmentDef);
  strip.getCoord(0);
  for (uint16_t p = 0; p < 7; p++) {
    coord = strip.getCoord();
    Serial.printf("Coord(%u) = {%3.2f, %3.2f}\n", p, coord.x, coord.y);
  }

  // myStrip[0].define(&stripDef[0]);
  // myStrip[1].define(&stripDef[1]);
  //   myStrip[1].getPos(0);
  // for (uint8_t i = 0; i < 8; i++) {
  //   pos = myStrip[1].getPos();
  //   Serial.printf("pos[%i] = %3.2f\n", i, pos);
  // }
  // mySine.SetStepPeriod(framePeriod);
  // delay(3000);
  // mySine.update(0.5, 1.0, 0.5, 0);
  // mySine.update(0.0, 1.0, 0.5, 1.0);
}


void loop() {
  // if (frameTimer >= framePeriod) {  // check for start of new frame
  //   frameTimer = 0;   // restart timer immediately for next frame
  //   mySine.step();
  //   if (count++ < 20) {
  //     Serial.printf("angle = %4.3f offset = %4.3f ampl = %4.3f, sine = %4.3f\n", 
  //         mySine.phaseAngle, mySine.offset, mySine.amplitude, mySine.value());
  //   }
  // }
}

*/