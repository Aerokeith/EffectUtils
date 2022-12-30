/* RANDOMIZER.CPP
    Implements the randomizerClass, which implements methods to randomly assign different "type" values to each pixel in an array.
    Type values range from 0 to N, where N is specified during object initialization. These methods can be used by callers to apply
    different effects to individual pixels withing an LED strip, where randomness is desired. Normally, the probabilty of each type 
    value is the same. Optionally, the probability of type value 0 may be specified (type0Prob), and then the remaining values have
    a probability of (1 - type0Prob) / (N - 1). 
    A method is provided to access the type value assigned to each pixel, which remain fixed until an explicit call to re-randomize
    the type values. This requires the class to dynamically allocate memory (1 byte per pixel) to store the type values. 
*/
#include <Arduino.h>
#include "Randomizer.h"


/* randomizerClass::init()
    Overload of start(), omitting the useShortestDist amd positiveDir parameters for backward compatibility.
  Parameters:
    uint16_t numPix: Number of pixels to be randomized (e.g pixels in an LED strip)
    uint8_t numTyp: Number of different types values to be assigned, in range 0 - (numTyp - 1)
    float t0Prob: Probability of type value 0. (t0Prob == 0) disables this feature
  Returns: None
*/
void randomizerClass::init(uint16_t numPix, uint8_t numTyp, float t0Prob) {
  numPixels = numPix;   // save the parameters for later use
  numTypes = numTyp;
  type0Prob = t0Prob;
  pixType = new uint8_t [numPixels];  // allocate 1 byte per pixel
  randomize();  // perform initial randomization
}


/* randomizerClass::randomize()
    Uses the parameters provided in init() to randomize the type values stored in the pixel array allocated in init().
  Parameters: None
  Returns: None
*/
void randomizerClass::randomize() {
  if (pixType != NULL) {  // make sure memory allocation was successful
    for (uint16_t p = 0; p < numPixels; p++) {  // for each pixel in the allocated array
      if (type0Prob == 0) {             // if type 0 probability has not been specified
        pixType[p] = random(numTypes);  // randomy assign type, with equal probability for all type values
      }
      else {    // type 0 probability was specified
        if (random(100) < (uint32_t) (type0Prob * 100)) // use probability to determine if this pixel is type 0
          pixType[p] = 0;
        else {  // type 0 was not assigned to this pixel
          pixType[p] = random(1, numTypes); // randomly pick type from remaining values (> 0)
        }
      }
    }
  }
}


/* randomizerClass::getPixType()
    Return the type value for this pixel based on the values randomly assigned by previous call to randomize()
  Parameters: 
    uint16_t pixel: number of pixel in array
  Returns: 
    uint8_t: pixel type value (0 - (numTypes - 1))
*/
uint8_t randomizerClass::getPixType(uint16_t pixel) {
  if ((pixType == NULL) || (pixel >= numPixels))  // check paramneter range
    return (0);
  else
    return (pixType[pixel]);
}
