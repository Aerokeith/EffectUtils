#include <Arduino.h>

#ifndef _RANDOMIZER_TYPES   // prevent duplicate type definitions when this file is included in multiple places
#define _RANDOMIZER_TYPES

class randomizerClass {
  uint16_t numPixels;   // number of pixels to have random types assigned
  uint8_t numTypes;     // number of different pixel types to be randomly assigned (0 - (numTypes-1))
  uint8_t *pixType;     // pointer to dynamically-allocated pixel array
  float type0Prob;      // probability of each pixel being assigned type=0 (type0Prob=0 disables this feature)
public:
  randomizerClass() { pixType = NULL; }
  void init(uint16_t numPix, uint8_t numTyp, float t0Prob);
  void randomize();
  uint8_t getPixType(uint16_t pixel);
};


#endif  // _RANDOMIZER_TYPES