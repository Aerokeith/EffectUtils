/* ZAP.CPP
*/
#include <Arduino.h>
#include "EffectUtils.h"
#include "ColorUtilsHsi.h"
#include "Flow.h"
#include "Zap.h"


void zapClass::init(float pixelSpacing, float zapFadeScale, hsiF emberColor, float emberHoldScale,
                    float emberFadeScaleMin, float emberFreqMin, float emberFreqMax, float emberFilter, 
                    float emberMinVal, uint8_t *emberType) {
  params.pixelSpacing = pixelSpacing;
  params.zapFadeScale = zapFadeScale;
  params.emberColor = emberColor;
  params.emberHoldScale = emberHoldScale;
  params.emberFadeScaleMin = emberFadeScaleMin;
  params.emberFreqMin = emberFreqMin;
  params.emberFreqMax = emberFreqMax;
  params.emberFilter = emberFilter;
  params.emberMinVal = emberMinVal;
  params.emberType = emberType;
  for (uint8_t i = 0; i < numEmberTypes; i++)
    ember[i].init(&params);
  active = false;
}


/* zapClass::start()
    Starts the zap effect
  Parameters: 
  Returns: None
*/
void zapClass::start(hsiF zapColor, float duration, float speed, float distance) {

  zapCurColor = zapColor;
  flowDistance = distance;
  flowSpeed = speed;
  flowDuration = flowDistance / flowSpeed;
  emberDuration = max((duration - flowDuration), 0);   // remaining time for ember hold/fade
  effectSteps = ComputeSteps(duration);  // total number of steps in ZAP effect
  stepNum = 0;
  zapFlow.start(flowDuration, distance, 0);
  active = true;
}


/* zapClass::step()
    Called once per step period (frame) to update zap effect, if active
  Parameters: None
  Returns: None
*/
void zapClass::step() {
  if (active) {
    stepNum++;
    zapFlow.step();
    for (uint8_t i = 0; i < numEmberTypes; i++)
      ember[i].step();
    if (zapFlow.completed()) {
      if (emberDuration > 0) {
        zapFade.start((emberDuration * params.zapFadeScale), &zapCurColor, zapOffColor);
        for (uint8_t i = 0; i < numEmberTypes; i++) 
          ember[i].start(emberDuration);
      }
    }
    else {
      zapFade.step();
    }
    if (stepNum >= effectSteps) {  // if flow is done
      active = false;
    }
  }
}


hsiF zapClass::colorVal(uint16_t pixel) {
  hsiF retVal;
  uint8_t pixelType;

  if (!active)
    return (zapOffColor);
  pixelType = params.emberType[pixel];
  if (pixelType == 0) {
    retVal = zapCurColor;
    if (zapFlow.active) 
      retVal.i *= zapFlow.val(pixel * params.pixelSpacing);
  }
  else {
    retVal = params.emberColor;
    if (zapFlow.active) 
      retVal.i *= zapFlow.val(pixel * params.pixelSpacing);
    else if (ember[pixelType-1].active) {
      retVal.i *= ember[pixelType-1].val();
      // Serial.printf("EmberVal= %3.2f\n", ember[pixelType-1].val());   // DEBUG
    }
    else
      retVal.i = 0;
  }
  return (retVal);  
}




void emberClass::init(zapConfigStruct *params) {
  frequency = (float) random((uint32_t) (params->emberFreqMin * 100), (uint32_t) (params->emberFreqMax * 100) + 1) / 100;
  fadeScale = (float) random((uint32_t) (params->emberFadeScaleMin * 100), 101) / 100;
  holdScale = params->emberHoldScale;
  filter = params->emberFilter;
  minVal = params->emberMinVal;
  flicker.active = false;
  active = false;
}


void emberClass::start(float maxEmberDuration) {
  float rampDownDuration;;
  float holdDuration;

  holdDuration = maxEmberDuration * holdScale;
  rampDownDuration = (maxEmberDuration = holdDuration) * fadeScale;
  flicker.setRamp(0, rampDownDuration);
  flicker.start((holdDuration + rampDownDuration), frequency, filter, minVal);  
  active = true;
}


void emberClass::step() {
  flicker.step();
  active = flicker.active;
}


float emberClass::val() {
  return (flicker.val());
}