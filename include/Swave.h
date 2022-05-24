#include <Arduino.h>
#include "EffectUtils.h"
#include "Ramp.h"

#ifndef _SWAVE_TYPES  // prevent duplicate type definitions when this file is included in multiple places
#define _SWAVE_TYPES

class swaveClass : public effect {    // derived from "effect" class defined in EffectUtils.h
  float amplitude;      // maximum amplitude of wave (0 - 1)
  float waveLength;     // wavelength in mm
  float phaseAngle;      // current wave phase angle
  float phaseDelta;      // phase angle change per step
  rampClass ramp;       // embedded ramp object in each wave instance
public:
  swaveClass() { active = false; }   // object constructor
  void start(float duration, float wavelen, float freq, float ampl);
  void setRamp(float rampDur);
  void step();
  float value(float position);  // value of wave function (-amplitude to +amplitude) at specified offset from origin (mm)
};

#endif  // _SWAVE_TYPES
