//
//  PreADCChain.cpp
//  s1995
//
//  Created by giulio iacomino on 23/01/2025.
//

#include "PreADCChain.hpp"



void PreADCChain::prepare(const juce::dsp::ProcessSpec& spec){
    gain.prepare(spec);
    gain.setRampDurationSeconds(0.01);
    gain.setGainLinear(0.1f);
    gain.reset();


    saturation.prepare(spec);
    saturation.functionToUse = [](float x) {
        float positive = std::tanh(x * 1.05f);  // positive = soft
        float negative = x * 1.05f - 0.2f * x * x * x;  // negative = hard
        return (x > 0.0f) ? positive : negative;
    };
    

    dcFilter.prepare(spec);
    *dcFilter.state = *juce::dsp::IIR::Coefficients<float>::makeHighPass(spec.sampleRate, 20.0f);
    dcFilter.reset();
}

void PreADCChain::process(juce::dsp::AudioBlock<float>& block){

    const float gainDB = *parameters.getRawParameterValue("inputGain");
    gain.setGainDecibels(gainDB);
    
    gain.process(juce::dsp::ProcessContextReplacing<float>(block));
    saturation.process(juce::dsp::ProcessContextReplacing<float>(block));
    dcFilter.process(juce::dsp::ProcessContextReplacing<float>(block));
}
