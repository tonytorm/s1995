//
//  ADCChain.hpp
//  s1995
//
//  Created by giulio iacomino on 22/01/2025.
//

#ifndef ADCChain_hpp
#define ADCChain_hpp

#include <stdio.h>
#include <JuceHeader.h>

class ADCChain
{
public:
    void prepare(juce::dsp::ProcessSpec& spec);
    void processBlock(juce::dsp::AudioBlock<float> block);
    void setCutoff(float cutoffHz) { currentCutoff = cutoffHz; };
    void setDrive(float driveAmount) { drive = driveAmount; };

private:
    juce::dsp::ProcessorChain<juce::dsp::IIR::Filter<float>, juce::dsp::IIR::Filter<float>, juce::dsp::IIR::Filter<float>> butterworthLeft;
    juce::dsp::ProcessorChain<juce::dsp::IIR::Filter<float>, juce::dsp::IIR::Filter<float>, juce::dsp::IIR::Filter<float>> butterworthRight;
    
    const int bitDepth = 12;
    float totalQLevels = -1.f;

    float Q1 = 0.5175;
    float Q2 = 0.7071;
    float Q3 = 1.9319;
    
    
    int sampleRate = 0;
    float currentCutoff = 0.f;
    float drive = 1.0f;
    juce::Random thermalNoise;
};

#endif /* ADCChain_hpp */
