# S1995 - 90s-Style ADC/DAC Sampler Emulator

A  VST3 plugin that emulates the characteristics of old-school ADC/DAC sampler converters, loosely inspired by the S950. Designed and built through a combination of techniques:

![](./Assets/image.png)

## Implementation Details

### Anti-Aliasing Filter
A **6th order Butterworth** filter is applied to emulate the frequency response of vintage samplers and prevent aliasing during the sampling process.

### Bit Depth Reduction
The signal undergoes bit depth reduction, introducing quantization artifacts that mimic the resolution limits of classic samplers.

### Sample Rate Decimation
The divide-rate technique is used for sample rate reduction.

#### Planned Features

Variable Sample Rate Reduction: A parameter to adjust the degree of decimation dynamically, offering more control over the lo-fi effect.

**The project is a personal exploration into audio processing and vintage sampler emulation.**
