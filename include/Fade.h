#include <Arduino.h>
#include "ColorUtilsHsi.h"
#include "EffectUtils.h"


#ifndef _FADE_TYPES  // prevent duplicate type definitions when this file is included in multiple places
#define _FADE_TYPES


class fadeClass : public effect {
  hsiF *fadeColorPtr;  // pointer to the color to be faded
  hsiF endColor;    // target color at end of fade duration
  hsiF delta;       // color change per fade step
public:
  fadeClass() { active = false; }
  void start(float duration, hsiF *curColor, hsiF targetColor);
  void step();
};

#endif  // _FADE_TYPES

