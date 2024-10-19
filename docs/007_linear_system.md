# Linear Systems in Digital Signal Processing

## Conventions

1. Input signal: x[n]
2. Output signal: y[n]
3. System response: h[n]
4. Discrete-time variable: n

## Definition

A system is linear if it satisfies two properties:
1. Additivity
2. Homogeneity (Scaling)

## Properties of Linear Systems

### 1. Superposition Principle

For inputs x1[n] and x2[n] with corresponding outputs y1[n] and y2[n]:

T{a1x1[n] + a2x2[n]} = a1y1[n] + a2y2[n]

Where T{} represents the system operation, and a1 and a2 are scalar constants.

### 2. Time-Invariance

If y[n] is the response to x[n], then y[n-k] is the response to x[n-k] for any integer k.

### 3. Memoryless Property

The output at any time depends only on the input at that time:

y[n] = f(x[n])

### 4. Causality

The output depends only on past and present inputs, not future inputs:

y[n] = f(x[n], x[n-1], x[n-2], ...)

### 5. Stability

A bounded input produces a bounded output (BIBO stability):

|x[n]| ≤ M < ∞ for all n → |y[n]| ≤ K < ∞ for all n

Where M and K are finite constants.

### 6. Invertibility

If distinct inputs produce distinct outputs, the system is invertible.

### 7. Distributive Property

T{x1[n] * x2[n]} = T{x1[n]} * T{x2[n]}

Where * denotes convolution.

### 8. Commutative Property

For linear time-invariant (LTI) systems:

x[n] * h[n] = h[n] * x[n]

### 9. Associative Property

For LTI systems:

(x[n] * h1[n]) * h2[n] = x[n] * (h1[n] * h2[n])

### 10. Shift Property

For LTI systems:

If y[n] = T{x[n]}, then y[n-k] = T{x[n-k]}


## More on Superposition Theorem
----------------------------
# Superposition Theorem in Linear Systems

## Introduction

The superposition theorem is a fundamental principle in linear systems theory, including digital signal processing. It states that for a linear system, the response to a sum of inputs is equal to the sum of the responses to each input applied separately.

## Formal Definition

For a linear system T{}, given inputs x₁[n] and x₂[n] with corresponding outputs y₁[n] and y₂[n]:

T{a₁x₁[n] + a₂x₂[n]} = a₁T{x₁[n]} + a₂T{x₂[n]} = a₁y₁[n] + a₂y₂[n]

Where a₁ and a₂ are scalar constants.

## Components of Superposition

The superposition theorem is based on two fundamental properties of linear systems:

1. **Additivity**: T{x₁[n] + x₂[n]} = T{x₁[n]} + T{x₂[n]}
2. **Homogeneity** (Scaling): T{ax[n]} = aT{x[n]}, where 'a' is a scalar

## Implications and Applications

1. **Simplification of Complex Inputs**: 
   - Complex input signals can be decomposed into simpler components.
   - The system's response to each component can be analyzed separately.
   - The overall response is the sum of individual responses.

2. **Analysis of Linear Time-Invariant (LTI) Systems**:
   - In LTI systems, the impulse response h[n] fully characterizes the system.
   - The output y[n] for any input x[n] can be computed using convolution:
     y[n] = x[n] * h[n]

3. **Frequency Domain Analysis**:
   - Supports the concept of transfer functions in the frequency domain.
   - Allows for the analysis of systems using Fourier and Z-transforms.

4. **Circuit Analysis**:
   - In electrical circuits, it allows for the analysis of complex circuits by considering one source at a time.

5. **Signal Decomposition**:
   - Enables techniques like Fourier series, where complex signals are represented as sums of simpler sinusoidal components.

## Example

Consider a linear system T{} and two inputs:
- x₁[n] = [1, 2, 3, 4]
- x₂[n] = [5, 6, 7, 8]

Let's say the system doubles the input:
- y₁[n] = T{x₁[n]} = [2, 4, 6, 8]
- y₂[n] = T{x₂[n]} = [10, 12, 14, 16]

Now, consider the input x[n] = 2x₁[n] + 3x₂[n] = [17, 22, 27, 32]

By superposition:
T{x[n]} = T{2x₁[n] + 3x₂[n]} = 2T{x₁[n]} + 3T{x₂[n]} = 2y₁[n] + 3y₂[n]
        = 2[2, 4, 6, 8] + 3[10, 12, 14, 16]
        = [4, 8, 12, 16] + [30, 36, 42, 48]
        = [34, 44, 54, 64]

This is indeed double the input x[n], confirming the superposition principle.

## Limitations

- Applies only to linear systems
- Does not hold for nonlinear systems (e.g., systems with saturation, hysteresis, etc.)

## Conclusion

The superposition theorem is a powerful tool in the analysis and design of linear systems. It allows complex problems to be broken down into simpler, manageable parts, making it an essential concept in fields ranging from signal processing to control systems and beyond.
