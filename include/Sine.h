#include <Arduino.h>
#include "EffectUtils.h"
#include "RampVar.h"

#ifndef _SINE_TYPES  // prevent duplicate type definitions when this file is included in multiple places
#define _SINE_TYPES

class sineClass : public effect {    // derived from "effect" class defined in EffectUtils.h
public:
  float offset;        // positive offset of sine wave baseline from 0 (0 - 1)
  float amplitude;      // absolute value of sine wave amplitude relative to offset baseline (0 - 1)
  float frequency;     // sine frequency (Hz)
  float phaseAngle;      // current wave phase angle (radians)
  float phaseDelta;      // phase angle change (radians) per step
  rampVarClass freqRamp;       // embedded rampVar object to ramp frequency
  rampVarClass offsetRamp;    // embedded rampVar object to ramp offset
  rampVarClass amplitudeRamp; // embedded rampVar object to ramp amplitude
  sineClass() { active = false; }   // object constructor
  void update(float freq, float level,  float ampl, float rampDur);
  void step();
  float value();  // current value of sine wave function
  float value(float phaseOffsetFrac);  // current value of sine wave function at a specified phase offset
};

#endif  // _SINE_TYPES
