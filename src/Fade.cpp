/* FADE.CPP
  This module defines the fadeClass, which implements a linear fade from a "current" color to a "target" color, where both colors are 
  HSI color variables of type hsiF, as defined in ColorUtils.h A pointer to the current color is provided as a parameter to the
  fadeClass::start() function, and subsequent calls to fadeClass::step() will update this variable during the fade effect. 
  If the initial value of this color is "off" (i == 0), the hue and saturation values are forced to be the same as the target color, 
  so that the fade-in is in brightness only. Similarly, if the target color is "off", the hue and saturation values of the target color 
  are forced to be the same as the initial value of the current color, so that the fade-out is in brightness only. 
*/
#include <Arduino.h>
#include "EffectUtils.h"
#include "Fade.h"


/* fadeClass::start()
    Starts the fade effect. 
  Parameters:
    float duration: Total duration of the effect (in seconds). 
    hsiF *curColor: Pointer to the color value to be faded
    hsiF targetColor: The desired color at the end of the fade duration
  Returns: None
*/
void fadeClass::start(float duration, hsiF *curColor, hsiF targetColor) {

  fadeColor = curColor;   // copy pointer to class data member
  if (fadeColor->i < 0.1) {     // if LED is currently off (or very close to off)
    fadeColor->h = targetColor.h;  // then this is a brightness-only fade-in to targetColor
    fadeColor->s = targetColor.s;
  }
  else if (targetColor.i < 0.1) {    // if target color is off (or very close to off)
    targetColor.h = fadeColor->h;  // then this is a brightness-only fade-out
    targetColor.s = fadeColor->s;
  }
  endColor = targetColor;   
  effectSteps = ComputeSteps(duration);  // total number of steps in fade effect
  delta.h = HueDistance(fadeColor->h, endColor.h) / effectSteps;  // compute the HSI delta per step
  delta.s = (endColor.s - fadeColor->s) / effectSteps;
  delta.i = (endColor.i - fadeColor->i) / effectSteps;
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
    fadeColor->h += delta.h;
    fadeColor->s += delta.s;
    fadeColor->i += delta.i;
    stepNum++;
    if (stepNum >= effectSteps) {  // if fade is done
      *fadeColor = endColor;  // correct any potential rounding errors to ensure color ends up at endColor
      active = false;
    }
  }
}
