//
//  DacChain.cpp
//  s1995
//
//  Created by giulio iacomino on 25/01/2025.
//

#include "DACChain.hpp"

void DACChain::prepare(const juce::dsp::ProcessSpec& spec){
    gain.prepare(spec);
    gain.setRampDurationSeconds(0.01);
}

void DACChain::setGain(float gainValue){
    gain.setGainDecibels(gainValue);
}

void DACChain::process(juce::dsp::AudioBlock<float>& block){
    juce::dsp::ProcessContextReplacing<float> context(block);
    gain.process(context);
}
