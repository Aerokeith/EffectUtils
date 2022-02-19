/* FADE.CPP
  This module defines the fadeClass, which implements a linear fade between two specified colors over a specified duration. The current
  fade value is available in the class variable curColor, of type hsiF defined in ColorUtils.h. If the initial color is "off" 
  (startColor.i == 0), the hue and saturation values of startColor are forced to be the same as targetColor, so that the fade-in is in
  brightness only. Similarly, if the target color is "off" (targetColor.i == 0), the hue and saturation values of targetColor are forced
  to be the same as startColor, so that the fade-out is in brightness only. 
*/
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
