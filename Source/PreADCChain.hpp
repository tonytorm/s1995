//
//  PreADCChain.hpp
//  s1995
//
//  Created by giulio iacomino on 23/01/2025.
//

#ifndef PreADCChain_hpp
#define PreADCChain_hpp

#include <stdio.h>
#include <JuceHeader.h>

class PreADCChain
{
public:
    PreADCChain(juce::AudioProcessorValueTreeState& state) : parameters(state) {}
    void prepare(const juce::dsp::ProcessSpec& spec);
    void process(juce::dsp::AudioBlock<float>& block);
    

private:
    juce::dsp::Gain<float> gain;
    juce::dsp::WaveShaper<float> saturation;
    juce::dsp::ProcessorDuplicator<juce::dsp::IIR::Filter<float>, juce::dsp::IIR::Coefficients<float>> dcFilter;
    juce::AudioProcessorValueTreeState& parameters;
};

#endif /* PreADCChain_hpp */
