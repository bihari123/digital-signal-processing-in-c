# Signal Processing: Basic Statistical Measures

## Introduction to Signals

In simple terms: A signal is a list of numbers that change over time.

Technical definition: A signal is a function that conveys information about a phenomenon. In digital signal processing, we often work with discrete-time signals, represented as a sequence of values.

Example: Temperature readings every hour, audio samples in music, or pixel values in an image.

## 1. Mean (μ)

Simple explanation: The mean is the average of all the numbers in your signal.

Technical definition: The mean (μ) is the expected value of a signal, representing its DC (direct current) component.

Formula: μ = (1/N) * Σ(x[n])
Where:
- N is the total number of samples
- x[n] is the nth sample of the signal
- Σ denotes the sum over all samples

How to calculate:
1. Add up all the values in your signal
2. Divide by the total number of values

Example: For the signal [2, 4, 6, 8, 10]
Mean = (2 + 4 + 6 + 8 + 10) / 5 = 30 / 5 = 6

Why it's useful:
- Gives a "typical" value for your signal
- In signal processing: Represents the DC offset of the signal
- Used in signal centering: subtracting the mean from a signal removes its DC component

## 2. Variance (σ²)

Simple explanation: Variance shows how "spread out" your numbers are from the mean.

Technical definition: Variance (σ²) is the average of the squared differences from the mean, measuring the signal's AC power.

Formula: σ² = (1/N) * Σ((x[n] - μ)²)

How to calculate:
1. Subtract the mean from each value
2. Square these differences
3. Calculate the average of these squared differences

Example: For the signal [2, 4, 6, 8, 10] with mean = 6
Variance = ((2-6)² + (4-6)² + (6-6)² + (8-6)² + (10-6)²) / 5
         = (16 + 4 + 0 + 4 + 16) / 5 = 40 / 5 = 8

Why it's useful:
- Indicates how much a signal fluctuates
- In signal processing: Measures signal power or energy relative to DC component
- Used in estimating signal-to-noise ratio (SNR)

## 3. Standard Deviation (σ)

Simple explanation: Standard deviation is the square root of variance. It tells you, on average, how far values are from the mean.

Technical definition: Standard deviation (σ) is a measure of the amplitude variation of the signal.

Formula: σ = √(σ²)

How to calculate:
1. Calculate the variance
2. Take the square root of the variance

Example: Following from our variance calculation
Standard Deviation = √(Variance) = √8 ≈ 2.83

Why it's useful:
- Easier to interpret than variance as it's in the same units as the original signal
- In signal processing: Used for signal normalization and threshold setting
- Helps in detecting anomalies or significant changes in the signal

## Applications in Signal Processing

1. Signal Normalization
   Simple: Making all signals comparable by adjusting their scale.
   Technical: Transform a signal to have zero mean and unit variance.
   Process: x_norm[n] = (x[n] - μ) / σ

2. Signal-to-Noise Ratio (SNR) Estimation
   Simple: Comparing the level of desired signal to the level of background noise.
   Technical: SNR = 10 * log10(σ²_signal / σ²_noise) [in decibels]

3. Threshold Detection
   Simple: Deciding when a signal is "unusual" or "significant."
   Technical: Often set at μ ± kσ, where k is a chosen number of standard deviations.

4. Feature Extraction
   Simple: Describing a complex signal with a few key numbers.
   Technical: Mean and variance can serve as basic features in pattern recognition and machine learning algorithms.

5. Power Spectral Density Estimation
   Simple: Understanding how signal power is distributed across frequencies.
   Technical: Variance in the frequency domain relates to the power spectral density of the signal.
