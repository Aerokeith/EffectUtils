#include <Arduino.h>
#include "EffectUtils.h"

#ifndef _WAVELET_TYPES
#define _WAVELET_TYPES

const uint8_t WAVELETS_MAX_NUM = 8;         // max number of concurrently-active wavelets
const float WAVELET_DELAY_VAR = 0.5;        // random variation in the nominal inter-wavelet launch delay (+/-)
const float WAVELET_LENGTH_VAR = 0.5;       // random variation in the length of each wavelet (+/-)
const float WAVELET_START_VELOCITY = 0.1;   // fraction of maxVelocity to start each wavelet

struct waveletStruct {
  bool active;      // true if wavelet is active/running
  float velocity;    // current wavelet velocity (mm/step)
  float position;   // position (mm) of wavelet center, at point of max amplitude
  float length;     // wavelet wavelength
};

class waveletClass : public effect {   // derived from "effect" class
  float distance;     // travel distance of wavelets (mm)
  float maxVelocity;  // maximum velocity for all wavelets (mm/step)
  float acceleration; // acceleration for all wavelets (mm/step/step)
  float nomLength;       // nominal length of all wavelets (mm)
  float lengthVar;      // random variation (+/-) in wavelet length as a fraction of nomLength
  float nomDelay;     // nominal delay between successive wavelet launches
  float delayVar;     // random variation (+/-) in inter-wavelet delay as a fraction of nomDelay
  uint16_t launchStep;  // value of stepNum at which to launch next wavelet
  uint8_t nextWvl;    // number of next wavelet to be launched
  uint8_t lastWvl;    //number of last (previous) wavelet launched
  waveletStruct wvl[WAVELETS_MAX_NUM];  // array of per-wavelet structures
  void launch();
  float randomVar(float nomVal, float maxVar);
public:
  waveletClass() {active = false; lengthVar = WAVELET_DELAY_VAR; delayVar = WAVELET_DELAY_VAR; }
  void start(float duration, float dist, float speed, float accel, float len, float delay);
  void step();
  float val(float pos);
  void config(float lenVar, float dlyVar) { lengthVar = lenVar; delayVar = dlyVar; }
};

#endif // _WAVELET_TYPES