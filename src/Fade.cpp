#include <Arduino.h>
#include "EffectUtils.h"
#include "Fade.h"


/* fadeClass::start()
    Starts the fade effect. 
  Parameters:
    float duration: Total duration of the effect (in seconds). 
    hsiF startColor: The initial color to be faded
    hsiF targetColor: The desired color at the end of the fade duration
  Returns: None
*/
void fadeClass::start(float duration, hsiF startColor, hsiF targetColor) {

  if (startColor.i < 0.1) {     // if LED is currently off (or very close to off)
    startColor.h = targetColor.h;  // then this is a brightness-only fade-in to targetColor
    startColor.s = targetColor.s;
  }
  else if (targetColor.i < 0.1) {    // if target color is off (or very close to off)
    targetColor.h = startColor.h;  // then this is a brightness-only fade-out
    targetColor.s = startColor.s;
  }
  curColor = startColor;  // initialize the fadeClass parameters
  endColor = targetColor;   
  effectSteps = ComputeSteps(duration);  // total number of steps in fade effect
  delta.h = HueDistance(curColor.h, endColor.h) / effectSteps;  // compute the HSI delta per step
  delta.s = (endColor.s - curColor.s) / effectSteps;
  delta.i = (endColor.i - curColor.i) / effectSteps;
  stepNum = 0;
  active = true;
}


/* fadeClass::step()
    Called once per step period (frame) to update effect, if active
  Parameters: None
  Returns: None
*/
void fadeClass::step() {
  if (active) {   // if a fade is currently active (started, not finished)
    curColor.h += delta.h;
    curColor.s += delta.s;
    curColor.i += delta.i;
    stepNum++;
    if (stepNum >= effectSteps) {  // if fade is done
      curColor = endColor;  // correct any potential rounding errors to ensure color ends up at endColor
      active = false;
    }
  }
}
