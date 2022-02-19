#include <Arduino.h>
#include "EffectUtils.h"
#include "Ramp.h"

#ifndef _WAVE_TYPES  // prevent duplicate type definitions when this file is included in multiple places
#define _WAVE_TYPES

class waveClass : public effect {    // derived from "effect" class defined in EffectUtils.h
  float amplitude;      // maximum amplitude of sine wave (0 - 1)
  float phaseAngle;      // current sine wave phase angle at wave "origin"
  float phaseDelta;      // phase angle change per step
  rampClass ramp;       // embedded ramp object in each wave instance
public:
  float waveLength;   // wavelength supplied in start() for reference by calling functions
  waveClass() { active = false; }   // object constructor
  void start(float duration, float frequency, float ampl);
  void start(float duration, float wavelen, float speed, float ampl);
  void setRamp(float rampDur);
  void step();
  float val(float offset);  // value of wave function (0 - amplitude) at specified offset (fraction of wavelength)
  float val();  // value of wave function (0 - amplitude) at wave origin
};

#endif  // _WAVE_TYPES
