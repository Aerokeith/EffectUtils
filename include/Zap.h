#include <Arduino.h>
#include "EffectUtils.h"
#include "ColorUtilsHsi.h"
#include "Flow.h"
#include "Fade.h"
#include "Wave.h"
#include "Flicker.h"


#ifndef _ZAP_TYPES  // prevent duplicate type definitions when this file is included in multiple places
#define _ZAP_TYPES

const hsiF zapOffColor = {0, 1, 0};
const uint8_t numEmberTypes = 7;

struct zapConfigStruct {
  float pixelSpacing;         // distance (mm) between adjacent pixels
  float zapFadeScale;         // % of post-flow duration over which the zap color fades out
  hsiF emberColor;            // ember color, including brightness
  float emberHoldScale;       // % of post-flow duration over which ember flicker is held prior to fading
  float emberFadeScaleMin;    // Min % of remaining duration (after flow & hold) over which embers fade out (max is 100%)
  float emberFreqMin;         // Min ember flicker frequency
  float emberFreqMax;         // Max ember flicker frequency
  float emberFilter;          // Filter value for the flicker effect (see effectUtils::flicker)
  float emberMinVal;          // Minimum ember flicker brightness
  uint8_t *emberType;         // pointer to a array defining the type of ember (0 - numZapEmberTypes) for each pixel 
};


class emberClass {
  flickerClass flicker;
  float frequency;
  float holdScale;
  float fadeScale;
  float filter;
  float minVal;
public:
  bool active;
  emberClass() { active = false; }
  void init(zapConfigStruct *params);
  void start(float maxEmberDuration);
  void step();
  float val();
};


class zapClass : public effect {
  flowClass zapFlow;
  fadeClass zapFade;
  emberClass ember[numEmberTypes];
  hsiF zapCurColor;
  float emberDuration;
  float flowDistance;
  float flowSpeed;
  float flowDuration;
public:
  zapConfigStruct params;
  void init(float pixelSpacing, float zapFadeScale, hsiF emberColor, float emberHoldScale,
            float emberFadeScaleMin, float emberFreqMin, float emberFreqMax, float emberFilter, 
            float emberMinVal, uint8_t *emberType);
  void start(hsiF zapColor, float duration, float speed, float distance);
  void step();
  hsiF colorVal(uint16_t pixel);
};

#endif  // _ZAP_TYPES

