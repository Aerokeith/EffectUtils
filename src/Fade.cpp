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
    bool useShortestDist: If true, the fade will occur based on the shortest distance between the hues of targetColor and *curColor,
      with wrap-around if necessary. If false, the fade will be performed in the hue direction specified by positiveDir
    bool positiveDir: If true, the fade will be performed in the positive hue direction, with wrap-around if necessary
  Returns: None
*/
void fadeClass::start(float duration, hsiF *curColor, hsiF targetColor, bool useShortestDist, bool positiveDir) {

  fadeColorPtr = curColor;   // copy pointer to class data member
  if (fadeColorPtr->i < 0.1) {     // if LED is currently off (or very close to off)
    fadeColorPtr->h = targetColor.h;  // then this is a brightness-only fade-in to targetColor
    fadeColorPtr->s = targetColor.s;
  }
  else if (targetColor.i < 0.1) {    // if target color is off (or very close to off)
    targetColor.h = fadeColorPtr->h;  // then this is a brightness-only fade-out
    targetColor.s = fadeColorPtr->s;
  }
  endColor = targetColor;   
  effectSteps = ComputeSteps(duration);  // total number of steps in fade effect
  delta.h = HueDistance(fadeColorPtr->h, endColor.h, useShortestDist, positiveDir) / effectSteps;  // compute the HSI delta per step
  delta.s = (endColor.s - fadeColorPtr->s) / effectSteps;
  delta.i = (endColor.i - fadeColorPtr->i) / effectSteps;
  stepNum = 0;
  active = true;
}


/* fadeClass::start()
    Overload of start(), omitting the useShortestDist amd positiveDir parameters for backward compatibility.
  Parameters:
    float duration: Total duration of the effect (in seconds). 
    hsiF *curColor: Pointer to the color value to be faded
    hsiF targetColor: The desired color at the end of the fade duration
  Returns: None
*/
void fadeClass::start(float duration, hsiF *curColor, hsiF targetColor) {
  start(duration, curColor, targetColor, true, false);
}


/* fadeClass::step()
    Called once per step period (frame) to update effect, if active
  Parameters: None
  Returns: None
*/
void fadeClass::step() {
  if (active) {   // if a fade is currently active (started, not finished)
    fadeColorPtr->h += delta.h;
    if (fadeColorPtr->h > 1.0)  // handle wrap conditions
      fadeColorPtr->h -= 1.0;
    else if (fadeColorPtr->h < 0)
      fadeColorPtr->h += 1.0;
    fadeColorPtr->s += delta.s;
    fadeColorPtr->i += delta.i;
    stepNum++;
    if (stepNum >= effectSteps) {  // if fade is done
      *fadeColorPtr = endColor;  // correct any potential rounding errors to ensure color ends up at endColor
      active = false;
    }
  }
}
