#ifndef TOM_CAT_EFFECT_H
#define TOM_CAT_EFFECT_H

#define SAMPLE_RATE 44100
#define FRAMES_PER_BUFFER 512
#define NUM_SECONDS 10
#define NUM_CHANNELS 1

#define PITCH_SHIFT_FACTOR 1.5
#define MEOW_FREQ 500.0
#define MEOW_BANDWIDTH 100.0
#define DISTORTION_FACTOR 1.2

void applyTomCatEffect(float *buffer, int numSamples);

#endif // TOM_CAT_EFFECT_H
