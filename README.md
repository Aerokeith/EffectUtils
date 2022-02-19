# EffectUtils
Library of utilities for animation of LED lighting effects, consisting of the following modules:

EffectUtils: Defines a core  effectClass that is used by other modules to create derived classes

Ramp: Defines a rampClass that implements a trapezoidal ramp function (ramp-up, hold, ramp-down) that can be used to modulate the amplitude of other effects.

Wave: Defines a waveClass that implements a sine wave function that can be used to modulate LED brightness/color based on time and/or LED position

Flicker: Defines a flickerClass that implements a filtered random-step function that can be used to simulate flickering-flame effects

Wavelet: Defines a waveletClass that can be used to "launch" discrete sine wave-shaped "wavelets" over a specified distance, with a target speed and acceleration. Random variations are applied to a specified nominal wavelet length and inter-wavelet delay.
