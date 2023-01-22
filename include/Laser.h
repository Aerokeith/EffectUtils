#include <Arduino.h>
#include "EffectUtils.h"
#include "ColorUtilsHsi.h"
#include "Flow.h"
#include "RampVar.h"
#include "Flicker.h"
#include "Randomizer.h"

#ifndef _LASER_TYPES  // prevent duplicate type definitions when this file is included in multiple places
#define _LASER_TYPES

const uint8_t numEmberTypes = 4;
enum laserPhaseEnum {ZAP_PHASE, EMBER_PHASE};
const hsiF laserOffColor = {0, 1, 0};
const float flickerFilter = 0.99;
const float flickerMinVal = 0.5;

struct laserConfigStruct {
  float beamWidth;
  hsiF zapStartColor;
  hsiF zapEndColor;
  float emberDurScaleMin;
  float emberFreqMin;
  float emberFreqMax;
};

class laserClass : public effect {
  const laserConfigStruct *config;
  laserPhaseEnum phase;
  hsiF beamColor;
  uint16_t numPixels;
  float pixelSpacing;
  float zapLen;
  float emberDurMax;
  flowClass zapFlow;
  rampVarClass emberRamp;
  float emberScale;
  flickerClass emberFlicker[numEmberTypes];
  randomizerClass randomizer;
public:
  void init(uint16_t numPix, float distance, const laserConfigStruct *configParams);
  void start(hsiF laserColor, float zapDur, float duration);
  void step();
  hsiF colorVal(uint16_t pixel);
};

#endif    // _LASER_TYPES