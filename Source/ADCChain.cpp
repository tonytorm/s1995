//
//  ADCChain.cpp
//  s1995
//
//  Created by giulio iacomino on 22/01/2025.
//

#include "ADCChain.hpp"


void ADCChain::prepare(juce::dsp::ProcessSpec& spec){
    
    auto coeffs1 = juce::dsp::IIR::Coefficients<float>::makeLowPass(
        spec.sampleRate,
        currentCutoff,
        Q1
    );
    auto coeffs2 = juce::dsp::IIR::Coefficients<float>::makeLowPass(
        spec.sampleRate,
        currentCutoff,
        Q2
    );
    auto coeffs3 = juce::dsp::IIR::Coefficients<float>::makeLowPass(
        spec.sampleRate,
        currentCutoff,
        Q3
    );
    
    butterworthLeft.prepare(spec);
    butterworthRight.prepare(spec);
    
    butterworthLeft.get<0>().coefficients = coeffs1;
    butterworthLeft.get<1>().coefficients = coeffs2;
    butterworthLeft.get<2>().coefficients = coeffs3;
    
    butterworthRight.get<0>().coefficients = coeffs1;
    butterworthRight.get<1>().coefficients = coeffs2;
    butterworthRight.get<2>().coefficients = coeffs3;
    
    butterworthLeft.reset();
    butterworthRight.reset();
    
    totalQLevels = powf(2, bitDepth);
    divideRateCounters.resize(spec.numChannels, 0);
    
}

void ADCChain::processBlock(juce::dsp::AudioBlock<float> inputBlock){    
    
   
    
    auto leftBlock = inputBlock.getSingleChannelBlock(0);
    auto rightBlock = inputBlock.getSingleChannelBlock(1);
    juce::dsp::ProcessContextReplacing<float> leftContext(leftBlock);
    juce::dsp::ProcessContextReplacing<float> rightContext(rightBlock);
    
    butterworthLeft.process(leftContext);
    butterworthRight.process(rightContext);
    
    for (int channel = 0; channel < inputBlock.getNumChannels(); ++channel) {
        auto* channelData = inputBlock.getChannelPointer(channel);
        float heldSample = 0.0f;
        
        int& counter = divideRateCounters[channel];
        
        for (int sample = 0; sample < inputBlock.getNumSamples(); ++sample) {
            if (counter % downsampleFactor == 0) {
                float val = channelData[sample];
                val = std::round(val * totalQLevels) / totalQLevels;
                heldSample = val;
            }
            
            channelData[sample] = heldSample;
            counter++;
        }
    }
    
}
