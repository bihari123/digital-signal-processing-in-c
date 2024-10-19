#include <math.h>
#include <portaudio.h>
#include <stdio.h>
#include <stdlib.h>

#define SAMPLE_RATE 44100
#define FRAMES_PER_BUFFER 256
#define DURATION 1.0 // Duration of the meow in seconds

#define FREQUENCY_START 800.0
#define FREQUENCY_END 400.0
#define ATTACK_TIME 0.1
#define DECAY_TIME 0.2
#define SUSTAIN_LEVEL 0.7
#define RELEASE_TIME 0.3

typedef struct {
  float phase;
  int frame_index;
  int max_frames;
} paTestData;

static int patestCallback(const void *inputBuffer, void *outputBuffer,
                          unsigned long framesPerBuffer,
                          const PaStreamCallbackTimeInfo *timeInfo,
                          PaStreamCallbackFlags statusFlags, void *userData) {
  paTestData *data = (paTestData *)userData;
  float *out = (float *)outputBuffer;
  unsigned long i;

  (void)inputBuffer; // Prevent unused variable warning

  for (i = 0; i < framesPerBuffer; i++) {
    float time = (float)data->frame_index / SAMPLE_RATE;
    float frequency =
        FREQUENCY_START + (FREQUENCY_END - FREQUENCY_START) * time / DURATION;

    // ADSR envelope
    float envelope;
    if (time < ATTACK_TIME) {
      envelope = time / ATTACK_TIME;
    } else if (time < ATTACK_TIME + DECAY_TIME) {
      envelope =
          1.0f - (1.0f - SUSTAIN_LEVEL) * (time - ATTACK_TIME) / DECAY_TIME;
    } else if (time < DURATION - RELEASE_TIME) {
      envelope = SUSTAIN_LEVEL;
    } else {
      envelope = SUSTAIN_LEVEL *
                 (1.0f - (time - (DURATION - RELEASE_TIME)) / RELEASE_TIME);
    }

    // Generate sine wave
    *out++ = sinf(data->phase) * envelope * 0.5f;
    data->phase += 2.0f * M_PI * frequency / SAMPLE_RATE;
    if (data->phase >= 2.0f * M_PI)
      data->phase -= 2.0f * M_PI;

    data->frame_index++;

    if (data->frame_index >= data->max_frames)
      return paComplete;
  }

  return paContinue;
}

int main(void) {
  PaStream *stream;
  PaError err;
  paTestData data;
  int i;

  data.max_frames = SAMPLE_RATE * DURATION;
  data.frame_index = 0;
  data.phase = 0;

  err = Pa_Initialize();
  if (err != paNoError)
    goto error;

  err = Pa_OpenDefaultStream(&stream,
                             0,         // No input channels
                             1,         // Mono output
                             paFloat32, // 32 bit floating point output
                             SAMPLE_RATE, FRAMES_PER_BUFFER, patestCallback,
                             &data);
  if (err != paNoError)
    goto error;

  err = Pa_StartStream(stream);
  if (err != paNoError)
    goto error;

  printf("Meowing for %f seconds ...\n", DURATION);
  Pa_Sleep(DURATION * 1000);

  err = Pa_StopStream(stream);
  if (err != paNoError)
    goto error;

  err = Pa_CloseStream(stream);
  if (err != paNoError)
    goto error;

  Pa_Terminate();
  printf("Meow finished.\n");

  return 0;

error:
  Pa_Terminate();
  fprintf(stderr, "An error occurred while using the portaudio stream\n");
  fprintf(stderr, "Error number: %d\n", err);
  fprintf(stderr, "Error message: %s\n", Pa_GetErrorText(err));
  return -1;
}
