#include "../include/tom_cat_effect.h"
#include <math.h>
#include <portaudio.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

typedef struct {
  float *recordedSamples;
  int frameIndex;
  int maxFrameIndex;
} paTestData;

static int recordCallback(const void *inputBuffer, void *outputBuffer,
                          unsigned long framesPerBuffer,
                          const PaStreamCallbackTimeInfo *timeInfo,
                          PaStreamCallbackFlags statusFlags, void *userData) {
  paTestData *data = (paTestData *)userData;
  const float *rptr = (const float *)inputBuffer;
  float *wptr = &data->recordedSamples[data->frameIndex * NUM_CHANNELS];
  long framesToCalc = framesPerBuffer;
  long i;

  if (data->frameIndex + framesToCalc > data->maxFrameIndex) {
    framesToCalc = data->maxFrameIndex - data->frameIndex;
  }

  for (i = 0; i < framesToCalc; i++) {
    *wptr++ = *rptr++;
  }
  data->frameIndex += framesToCalc;

  return paContinue;
}

static int playCallback(const void *inputBuffer, void *outputBuffer,
                        unsigned long framesPerBuffer,
                        const PaStreamCallbackTimeInfo *timeInfo,
                        PaStreamCallbackFlags statusFlags, void *userData) {
  paTestData *data = (paTestData *)userData;
  float *wptr = (float *)outputBuffer;
  unsigned int i;

  for (i = 0; i < framesPerBuffer; i++) {
    if (data->frameIndex < data->maxFrameIndex) {
      *wptr++ = data->recordedSamples[data->frameIndex++];
    } else {
      *wptr++ = 0;
    }
  }

  return paContinue;
}

void applyTomCatEffect(float *buffer, int numSamples) {
  float *tempBuffer = (float *)malloc(numSamples * sizeof(float));
  memcpy(tempBuffer, buffer, numSamples * sizeof(float));

  // Pitch shifting
  float phase = 0;
  float phaseIncrement = PITCH_SHIFT_FACTOR;

  // Meow resonance filter
  float b0 = 0, b1 = 0, b2 = 0, a0 = 0, a1 = 0, a2 = 0;
  float w0 = 2 * M_PI * MEOW_FREQ / SAMPLE_RATE;
  float alpha = sin(w0) * sinh(log(2) / 2 * MEOW_BANDWIDTH * w0 / sin(w0));

  a0 = 1 + alpha;
  b0 = alpha / a0;
  b1 = 0;
  b2 = -alpha / a0;
  a1 = -2 * cos(w0) / a0;
  a2 = (1 - alpha) / a0;

  float x1 = 0, x2 = 0, y1 = 0, y2 = 0;

  for (int i = 0; i < numSamples; i++) {
    // Pitch shift
    int readIndex = (int)phase;
    float frac = phase - readIndex;
    float sample = (readIndex < numSamples - 1)
                       ? tempBuffer[readIndex] * (1 - frac) +
                             tempBuffer[readIndex + 1] * frac
                       : tempBuffer[readIndex];
    phase += phaseIncrement;
    if (phase >= numSamples)
      phase -= numSamples;

    // Apply meow resonance filter
    float x0 = sample;
    float y0 = b0 * x0 + b1 * x1 + b2 * x2 - a1 * y1 - a2 * y2;
    x2 = x1;
    x1 = x0;
    y2 = y1;
    y1 = y0;

    // Add some distortion for "growl"
    sample = tanh(y0 * DISTORTION_FACTOR) * 0.5;

    buffer[i] = sample;
  }

  free(tempBuffer);
}

int main(void) {
  PaStream *stream;
  PaError err;
  paTestData data;
  int i, totalFrames, numSamples, numBytes;
  time_t start_time, current_time;

  data.maxFrameIndex = totalFrames = NUM_SECONDS * SAMPLE_RATE;
  numSamples = totalFrames * NUM_CHANNELS;
  numBytes = numSamples * sizeof(float);
  data.recordedSamples = (float *)malloc(numBytes);
  if (data.recordedSamples == NULL) {
    printf("Could not allocate record array.\n");
    exit(1);
  }
  for (i = 0; i < numSamples; i++)
    data.recordedSamples[i] = 0;

  err = Pa_Initialize();
  if (err != paNoError)
    goto error;

  // Record
  err = Pa_OpenDefaultStream(&stream, NUM_CHANNELS, 0, paFloat32, SAMPLE_RATE,
                             FRAMES_PER_BUFFER, recordCallback, &data);
  if (err != paNoError)
    goto error;

  data.frameIndex = 0;

  err = Pa_StartStream(stream);
  if (err != paNoError)
    goto error;

  printf("Recording for %d seconds...\n", NUM_SECONDS);
  start_time = time(NULL);
  while ((current_time = time(NULL)) - start_time < NUM_SECONDS) {
    Pa_Sleep(100);
    printf(".");
    fflush(stdout);
  }
  printf("\nFinished recording.\n");

  err = Pa_StopStream(stream);
  if (err != paNoError)
    goto error;

  err = Pa_CloseStream(stream);
  if (err != paNoError)
    goto error;

  printf("Applying Tom Cat effect...\n");
  applyTomCatEffect(data.recordedSamples, numSamples);

  // Playback
  data.frameIndex = 0;

  err = Pa_OpenDefaultStream(&stream, 0, NUM_CHANNELS, paFloat32, SAMPLE_RATE,
                             FRAMES_PER_BUFFER, playCallback, &data);
  if (err != paNoError)
    goto error;

  printf("Starting playback...\n");
  err = Pa_StartStream(stream);
  if (err != paNoError)
    goto error;

  while (data.frameIndex < data.maxFrameIndex) {
    Pa_Sleep(100);
  }

  err = Pa_StopStream(stream);
  if (err != paNoError)
    goto error;

  err = Pa_CloseStream(stream);
  if (err != paNoError)
    goto error;

  Pa_Terminate();
  printf("Playback finished.\n");

  free(data.recordedSamples);
  return 0;

error:
  Pa_Terminate();
  fprintf(stderr, "An error occurred while using the portaudio stream\n");
  fprintf(stderr, "Error number: %d\n", err);
  fprintf(stderr, "Error message: %s\n", Pa_GetErrorText(err));
  if (data.recordedSamples)
    free(data.recordedSamples);
  return -1;
}
