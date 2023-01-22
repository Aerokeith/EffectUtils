#include <Arduino.h>
#include "Laser.h"


void laserClass::init(uint16_t numPix, float length, const laserConfigStruct *configParams) {
  numPixels = numPix;
  zapLen = length;
  config = configParams;
  active = false;
  randomizer.init(numPixels, numEmberTypes, 0);
}


void laserClass::start(hsiF laserColor, float zapDur, float duration) {
  beamColor = laserColor;
  pixelSpacing = zapLen / (float) numPixels;
  emberDurMax = duration - zapDur;
  phase = ZAP_PHASE;
  effectSteps = ComputeSteps(duration);  // total number of steps in fade effect
  active = true;
  stepNum = 0;
  zapFlow.start(zapDur, zapLen, 0);
  randomizer.randomize();
}


void laserClass::step() {
  float flickerFreq;

  if (active) {
    if (phase == ZAP_PHASE) {
      zapFlow.step();
      if (zapFlow.completed()) {
        phase = EMBER_PHASE;
        emberScale = 1.0;
        emberRamp.start(&emberScale, 0, emberDurMax);
        for (uint8_t n = 0; n < numEmberTypes; n++) {
          flickerFreq = config->emberFreqMin + (((float) random(0, 101) / 100) * (config->emberFreqMax - config->emberFreqMin));
          emberFlicker[n].start(emberDurMax, flickerFreq, flickerFilter, flickerMinVal);
        }
      }
    }
    else {    // phase == EMBER_PHASE
      emberRamp.step();
      for (uint8_t n = 0; n < numEmberTypes; n++)
        emberFlicker[n].step();
    }
    stepNum++;
    if (stepNum >= effectSteps) {  // if LASER effect is done
      active = false;
    }
  }
}


hsiF laserClass::colorVal(uint16_t pixel) {
  float pixelPos;
  float distFromBeam;
  hsiF retColor;

  if (!active)
    return (laserOffColor);
  pixelPos = pixel * pixelSpacing;
  if (phase == ZAP_PHASE) {
    if (zapFlow.curPos < pixelPos)
      return (laserOffColor);
    distFromBeam = (zapFlow.curPos - config->beamWidth) - pixelPos;
    if (distFromBeam <= 0)
      return (beamColor);
  }
  else    // phase == EMBER_PHASE
    distFromBeam = zapLen - pixelPos;
  retColor = InterpHsi(config->zapStartColor, config->zapEndColor, (distFromBeam / zapLen));
  if (phase == EMBER_PHASE) {
    retColor.i *= emberFlicker[randomizer.getPixType(pixel)].val();
    retColor.i *= emberScale;
  }
  return (retColor);
}
