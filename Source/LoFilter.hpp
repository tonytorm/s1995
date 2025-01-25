//
//  Lofilter.hpp
//  s1995
//
//  Created by giulio iacomino on 25/01/2025.
//

#ifndef Lofilter_hpp
#define Lofilter_hpp

#include <stdio.h>
#include <JuceHeader.h>

class LoFilter
{
public:
    void prepare(const juce::dsp::ProcessSpec& spec);
    void process(juce::dsp::AudioBlock<float>& block);
    void setCutoffFrequency(float newCutoff);
    void updateCutoffFrequency();

    float cutoffMin = 100.f;
    float cutoffMax = 15000.f;
private:
    juce::dsp::ProcessorChain<juce::dsp::IIR::Filter<float>, juce::dsp::IIR::Filter<float>, juce::dsp::IIR::Filter<float>> butterworthLeft;
    juce::dsp::ProcessorChain<juce::dsp::IIR::Filter<float>, juce::dsp::IIR::Filter<float>, juce::dsp::IIR::Filter<float>> butterworthRight;

    std::atomic<float> cutoffFrequency{-1.f};
    float sampleRate = -1;
    float Q1 = 0.5175;
    float Q2 = 0.7071;
    float Q3 = 1.9319;
};

#endif /* Lofilter_hpp */
