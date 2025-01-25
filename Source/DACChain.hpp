//
//  DacChain.hpp
//  s1995
//
//  Created by giulio iacomino on 25/01/2025.
//

#ifndef DacChain_hpp
#define DacChain_hpp

#include <stdio.h>
#include <JuceHeader.h>

class DACChain
{
public:
    void prepare(const juce::dsp::ProcessSpec& spec);
    void setGain(float gainValue);
    void process(juce::dsp::AudioBlock<float>& block);

private:
    juce::dsp::Gain<float> gain;
};

#endif /* DacChain_hpp */
