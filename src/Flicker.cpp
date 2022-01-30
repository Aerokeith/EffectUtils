#include <Arduino.h>
#include "UtilConfig.h"
#include "Flicker.h"

char fstr[80];  // DEBUG


void flickerClass::setRamp(float rampTime) {
  ramp.setRamp(rampTime);
}


void flickerClass::start(float duration, float frequency, float filter) {
  flickSteps = (uint16_t) ceil(duration / STEP_PERIOD);
  frequency = max(frequency, 0.01); 
  maxDelta = (float) 1.0 - filter;  // maxDelta = 1.0 when (filter == 0)
  cycleSteps = (uint16_t) ceil(1 / (frequency * STEP_PERIOD));
  flickStepNum = 0;
  cycleStepNum = 0;
  flickVal = 0;
  ramp.start(duration);
  active = true;
}


void flickerClass::step() {
  float delta;

  if (active) {
    if (cycleStepNum == 0) {  // if beginning of new cycle
      targetVal = (float) random(101) / 100;  // get random value between 0.0 and 1.0
    }
    delta = targetVal - flickVal;  
    if (delta > 0) {                      // new targetVal is > current flickVal
      flickVal += min(delta, maxDelta);   // add delta to get there, constrained by maxDelta
    }
    else if (delta < 0) {   // new targetVal < current flickVal
      flickVal += max(delta, -maxDelta);  // add (degative) delta, constrained by -maxDelta
    }
    ramp.step();
    cycleStepNum++;
    if (cycleStepNum >= cycleSteps) {   // if cycle is done
      cycleStepNum = 0; // start new cycle in next step
    }
    flickStepNum++;
    if (flickStepNum >= flickSteps) {
      flickVal = 0;
      active = false;
    }
  }
}

float flickerClass::val() {
  if (active) {
    return (flickVal * ramp.val);
  }
  else 
    return (0);
}
