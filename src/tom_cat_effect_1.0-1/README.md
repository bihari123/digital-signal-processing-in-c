# Tom Cat Voice Effect Program Documentation

## Table of Contents
1. [Overview](#overview)
2. [Requirements](#requirements)
3. [Installation](#installation)
4. [Usage](#usage)
5. [Code Structure](#code-structure)
6. [Digital Signal Processing Concepts](#digital-signal-processing-concepts)
   - [Sampling and Quantization](#sampling-and-quantization)
   - [Pitch Shifting](#pitch-shifting)
   - [Digital Filtering](#digital-filtering)
   - [Nonlinear Processing](#nonlinear-processing)
7. [Customization](#customization)
8. [Troubleshooting](#troubleshooting)
9. [Further Reading](#further-reading)

## Overview

This program is designed to record audio input, apply a "Tom Cat" voice effect, and play back the processed audio. The effect aims to simulate the voice of Tom Cat from the Tom and Jerry cartoons by applying pitch shifting, resonant filtering, and subtle distortion. It serves as a practical example of various digital signal processing techniques applied in real-time audio processing.

## Requirements

- C compiler (e.g., GCC)
- PortAudio library
- Math library

## Installation

1. Ensure you have a C compiler installed on your system.
2. Install the PortAudio library. On most Unix-like systems, you can use:
   ```
   sudo apt-get install libasound-dev portaudio19-dev libportaudio2
   ```
3. Save the provided code in a file named `tom_cat_effect.c`.

## Usage

1. Compile the program:
   ```
   gcc tom_cat_effect.c -o tom_cat_effect -lportaudio -lm
   ```
2. Run the program:
   ```
   ./tom_cat_effect
   ```
3. The program will record audio for 5 seconds, apply the Tom Cat effect, and then play back the processed audio.

## Code Structure

The program is structured into several key components:

1. **Main Function**: Handles the overall flow of the program, including initialization, recording, effect application, and playback.

2. **Record Callback**: Called by PortAudio to process input audio data.

3. **Play Callback**: Called by PortAudio to provide output audio data.

4. **applyTomCatEffect Function**: Applies the voice effect to the recorded audio data.

## Digital Signal Processing Concepts

This program leverages several key digital signal processing (DSP) concepts to achieve the Tom Cat voice effect. Here's a detailed explanation of each:

### Sampling and Quantization

The program works with digital audio, which is a discrete representation of continuous analog sound waves. Two fundamental concepts are at play here:

1. **Sampling**: The process of measuring the amplitude of a continuous signal at discrete time intervals. In our program, this is represented by the `SAMPLE_RATE` constant (44100 Hz), meaning we take 44,100 measurements of the audio signal per second.

2. **Quantization**: The process of mapping a large set of input values to a smaller set of output values. In our case, we use floating-point numbers to represent audio samples, which provides a high degree of precision.

### Pitch Shifting

Pitch shifting is accomplished using a simple time-domain technique:

```c
int readIndex = (int)phase;
float frac = phase - readIndex;
float sample = (readIndex < numSamples - 1) 
    ? tempBuffer[readIndex] * (1 - frac) + tempBuffer[readIndex + 1] * frac
    : tempBuffer[readIndex];
phase += phaseIncrement;
if (phase >= numSamples) phase -= numSamples;
```

This code implements a technique called "resampling":

1. We read samples from the input buffer at a different rate than we write them.
2. The `phase` variable represents our position in the input buffer.
3. We use linear interpolation to estimate sample values between actual samples, providing smoother pitch shifting.
4. The `phaseIncrement` determines how much we move through the input buffer for each output sample. A value > 1 increases pitch, while a value < 1 decreases pitch.

### Digital Filtering

We implement a biquad filter, specifically a peaking EQ filter, to emphasize the "meow" frequency:

```c
float w0 = 2 * PI * MEOW_FREQ / SAMPLE_RATE;
float alpha = sin(w0) * sinh(log(2) / 2 * MEOW_BANDWIDTH * w0 / sin(w0));

a0 = 1 + alpha;
b0 = alpha / a0;
b1 = 0;
b2 = -alpha / a0;
a1 = -2 * cos(w0) / a0;
a2 = (1 - alpha) / a0;
```

This code calculates the coefficients for our digital filter:

1. `w0` is the angular frequency of our desired peak.
2. `alpha` is a parameter that determines the width of the peak.
3. The `a` and `b` coefficients define the filter's frequency response.

The filter is then applied in the time domain using the difference equation:

```c
float y0 = b0*x0 + b1*x1 + b2*x2 - a1*y1 - a2*y2;
```

This equation represents a recursive filter, where the output depends on both input and previous output values.

### Nonlinear Processing

We apply subtle distortion to add harmonics and create a "growl" effect:

```c
sample = tanh(y0 * DISTORTION_FACTOR) * 0.5;
```

The `tanh` function is used for "soft clipping":

1. It introduces harmonic distortion in a smooth, musical way.
2. It naturally limits the output amplitude, preventing harsh digital clipping.
3. The `DISTORTION_FACTOR` controls the intensity of the effect.

## Customization

You can customize the effect by modifying the following parameters in the code:

- `PITCH_SHIFT_FACTOR`: Controls the amount of pitch increase. Default is 1.5. Increase for a higher pitch, decrease for a lower pitch.
- `MEOW_FREQ`: Sets the center frequency for the resonant filter. Default is 500.0 Hz. Adjust to emphasize different frequencies in the "meow" sound.
- `MEOW_BANDWIDTH`: Adjusts the width of the frequency band emphasized by the filter. Default is 100.0 Hz. Increase for a broader effect, decrease for a more focused effect.
- `DISTORTION_FACTOR`: Controls the amount of nonlinear distortion applied. Default is 1.2. Increase for more distortion, decrease for a cleaner sound.

Experiment with these values to achieve different cat-like voice effects or to adapt the program for other voice transformations.

## Troubleshooting

1. **Compilation Errors**: 
   - Ensure all required libraries are installed.
   - Check that the PortAudio library is in your system's library path.
   - If you encounter "undefined reference" errors, make sure you're linking the PortAudio and math libraries with `-lportaudio -lm`.

2. **Runtime Errors**:
   - Make sure your system's audio input and output devices are properly configured.
   - Check that no other application is using the audio devices.
   - If you get a "stream error" message, try closing other audio applications or restarting your computer.

3. **Poor Audio Quality**:
   - Adjust the effect parameters for better results. For example, if the voice sounds too high-pitched, reduce the `PITCH_SHIFT_FACTOR`.
   - Ensure you're in a quiet environment for recording to minimize background noise.
   - If you hear clicking or popping sounds, try increasing the `FRAMES_PER_BUFFER` value to process larger chunks of audio at once.

4. **No Sound Output**:
   - Check your system's volume settings.
   - Ensure your output device is correctly selected in your system settings.
   - Try running the program with sudo if you suspect permission issues.

For further assistance or to report issues, please contact the program maintainer or open an issue in the project's repository if available.

## Further Reading

For those interested in delving deeper into DSP concepts:

1. "Digital Signal Processing: Principles, Algorithms and Applications" by John G. Proakis and Dimitris G. Manolakis
   - This comprehensive textbook covers a wide range of DSP topics, including detailed explanations of digital filters and spectral analysis.

2. "The Scientist and Engineer's Guide to Digital Signal Processing" by Steven W. Smith (available free online)
   - An excellent resource for beginners, offering intuitive explanations of DSP concepts with practical applications.

3. "Understanding Digital Signal Processing" by Richard G. Lyons
   - Known for its clear explanations, this book is particularly good for understanding the mathematical foundations of DSP.

4. "Designing Audio Effect Plug-Ins in C++" by Will Pirkle
   - While focused on C++, this book provides valuable insights into implementing audio effects, which can be adapted to our C program.

These resources provide comprehensive coverage of the DSP principles used in this project and many more advanced topics, allowing you to expand your understanding and potentially enhance the Tom Cat voice effect program.
