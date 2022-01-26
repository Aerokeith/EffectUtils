#include <Arduino.h>

#ifndef _WAVE_TYPES  // prevent duplicate type definitions when this file is included in multiple places
#define _WAVE_TYPES

enum phaseEnum {rampUp, hold, rampDown};

class rampClass {
  float rampTime;       // nominal ramp duration as set by setRamp()
  uint16_t rampSteps;   // number of FRAME_PERIOD steps in each up or down ramp (may be reduced based on total duration)
  uint16_t holdSteps;   // number of steps in hold period
  uint16_t stepNum;     // current step number
  phaseEnum phase;      // current ramp phase  
  float rampDelta;      // added to val each ramp step
  float stepPeriod;     // duration of each ramp step (seconds)
public:
  bool active;    // ramp function is active (any ramp phase)
  float val;      // current output value of ramp function (0 - 1)
  rampClass();
  void start(float duration);
  void start(float duration, float rampDur);
  void setRamp(float rampTime);
  void step();
  void setStep(uint16_t stepPerMs);
};


class waveClass {
  float amplitude;      // maximum amplitude of sine wave (0 - 1)
  uint16_t waveSteps;   // total number of FRAME_PERIOD steps in wave effect (0 = infinite duration)
  uint16_t stepNum;     // current step number
  float phaseAngle;      // current sine wave phase angle at wave "origin"
  float phaseDelta;      // phase angle change per step
  float stepPeriod;     // duration of each ramp step (seconds)
  rampClass ramp;       // embedded ramp object in each wave instance
public:
  bool active;    // wave function is active
  float waveLength;   // wavelength supplied in start() for reference by calling functions
  waveClass();
  void start(float duration, float frequency, float ampl);
  void start(float duration, float wavelen, float speed, float ampl);
  void setRamp(float rampTime);
  void step();
  float val(float offset);  // value of wave function (0 - amplitude) at specified offset (fraction of wavelength)
  float val();  // value of wave function (0 - amplitude) at wave origin
  void setStep(uint16_t stepPerMs);
};

#endif  // _WAVE_TYPES
