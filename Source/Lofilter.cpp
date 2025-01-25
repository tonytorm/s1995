//
//  Lofilter.cpp
//  s1995
//
//  Created by giulio iacomino on 25/01/2025.
//

#include "LoFilter.hpp"

void LoFilter::prepare(const juce::dsp::ProcessSpec& spec){
    butterworthLeft.prepare(spec);
    butterworthRight.prepare(spec);
    sampleRate = spec.sampleRate;
}

void LoFilter::process(juce::dsp::AudioBlock<float>& block){
    auto leftBlock = block.getSingleChannelBlock(0);
    auto rightBlock = block.getSingleChannelBlock(1);
    
    juce::dsp::ProcessContextReplacing<float> leftContext(leftBlock);
    juce::dsp::ProcessContextReplacing<float> rightContext(rightBlock);
    
    butterworthLeft.process(leftContext);
    butterworthRight.process(rightContext);
}

void LoFilter::setCutoffFrequency(float newCutoff){
    if(newCutoff != cutoffFrequency){
        if (newCutoff >= cutoffMin && newCutoff <= cutoffMax){
            cutoffFrequency.store(newCutoff);
            updateCutoffFrequency();
        }
    }
}

void LoFilter::updateCutoffFrequency(){
    const auto cutoff = cutoffFrequency.load();
    
    auto coefficients1 = juce::dsp::IIR::Coefficients<float>::makeLowPass(sampleRate, cutoff, Q1);
    auto coefficients2 = juce::dsp::IIR::Coefficients<float>::makeLowPass(sampleRate, cutoff, Q2);
    auto coefficients3 = juce::dsp::IIR::Coefficients<float>::makeLowPass(sampleRate, cutoff, Q3);
    
    *butterworthLeft.get<0>().coefficients = *coefficients1;
    *butterworthLeft.get<1>().coefficients = *coefficients2;
    *butterworthLeft.get<2>().coefficients = *coefficients3;
    
    *butterworthRight.get<0>().coefficients = *coefficients1;
    *butterworthRight.get<1>().coefficients = *coefficients2;
    *butterworthRight.get<2>().coefficients = *coefficients3;
}
