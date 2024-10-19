#include <math.h>
#include <portaudio.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define SAMPLE_RATE 44100
#define FRAMES_PER_BUFFER 512
#define NUM_SECONDS 5
#define NUM_CHANNELS 1

#define PI 3.14159265358979323846

// Busy street effect parameters
#define NOISE_LEVEL 0.1
#define REVERB_DELAY 0.05 // 50 ms delay for short reverb
#define REVERB_DECAY 0.5
#define TRAFFIC_RUMBLE_FREQ 100.0 // Low frequency for traffic rumble

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

float randf() { return (float)rand() / (float)RAND_MAX; }

void applyBusyStreetEffect(float *buffer, int numSamples) {
  float *tempBuffer = (float *)malloc(numSamples * sizeof(float));
  memcpy(tempBuffer, buffer, numSamples * sizeof(float));

  int delayBufferSize = (int)(REVERB_DELAY * SAMPLE_RATE);
  float *delayBuffer = (float *)calloc(delayBufferSize, sizeof(float));
  int delayIndex = 0;

  float trafficPhase = 0.0;

  for (int i = 0; i < numSamples; i++) {
    // Add background noise
    float noise = (randf() * 2 - 1) * NOISE_LEVEL;

    // Add traffic rumble (low-frequency sine wave)
    float trafficRumble = sin(trafficPhase) * NOISE_LEVEL * 0.5;
    trafficPhase += 2 * PI * TRAFFIC_RUMBLE_FREQ / SAMPLE_RATE;
    if (trafficPhase > 2 * PI)
      trafficPhase -= 2 * PI;

    // Simple reverb
    float reverbSample = delayBuffer[delayIndex];
    delayBuffer[delayIndex] = tempBuffer[i] + reverbSample * REVERB_DECAY;
    delayIndex = (delayIndex + 1) % delayBufferSize;

    // Combine effects
    buffer[i] =
        tempBuffer[i] * 0.7 + reverbSample * 0.3 + noise + trafficRumble;

    // Soft clipping to prevent oversaturation
    buffer[i] = tanh(buffer[i]);
  }

  free(tempBuffer);
  free(delayBuffer);
}

int main(void) {
  PaStream *stream;
  PaError err;
  paTestData data;
  int i, totalFrames, numSamples, numBytes;
  time_t start_time, current_time;

  srand(time(NULL)); // Initialize random seed

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

  printf("Applying busy street effect...\n");
  applyBusyStreetEffect(data.recordedSamples, numSamples);

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
