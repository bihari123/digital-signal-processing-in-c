#include <portaudio.h>
#include <sndfile.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define SAMPLE_RATE 44100
#define FRAMES_PER_BUFFER 512
#define NUM_SECONDS 5
#define NUM_CHANNELS 1

typedef struct {
  int frameIndex;
  int maxFrameIndex;
  float *recordedSamples;
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

int main(void) {
  PaStream *stream;
  PaError err;
  paTestData data;
  int i, totalFrames, numSamples, numBytes;
  SNDFILE *outfile;
  SF_INFO sfinfo;
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
    Pa_Sleep(100); // Sleep for 100ms
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

  Pa_Terminate();

  // Save the recorded data as a WAV file
  sfinfo.samplerate = SAMPLE_RATE;
  sfinfo.channels = NUM_CHANNELS;
  sfinfo.format = SF_FORMAT_WAV | SF_FORMAT_FLOAT;

  outfile = sf_open("output.wav", SFM_WRITE, &sfinfo);
  if (!outfile) {
    printf("Error opening output file.\n");
    return -1;
  }

  sf_write_float(outfile, data.recordedSamples, numSamples);
  sf_close(outfile);

  printf("Saved recording to output.wav\n");

  free(data.recordedSamples);
  return 0;

error:
  Pa_Terminate();
  fprintf(stderr, "An error occurred while using the portaudio stream\n");
  fprintf(stderr, "Error number: %d\n", err);
  fprintf(stderr, "Error message: %s\n", Pa_GetErrorText(err));
  return -1;
}
