//
//  AudioMeterSource.cpp
//  s1995
//
//  Created by giulio iacomino on 25/01/2025.
//

#include "AudioMeterProcessor.hpp"

void AudioMeterProcessor::reset(){
    peak = 0.0f;
    lastPeakTime = 0;
    rms = 0.f;
}

void AudioMeterProcessor::processBlock(const juce::dsp::AudioBlock<float> block){
    const int numChannels = (int)block.getNumChannels();
    const int numSamples = (int)block.getNumSamples();

    if (numChannels == 0 || numSamples == 0)
        return;

    float currentRms = 0.0f;
    float currentPeak = 0.0f;

    for (int ch = 0; ch < numChannels; ++ch){
        currentRms += getRMSLevel(block, ch, 0, numSamples);
        currentPeak = std::max(currentPeak, getMagnitude(block, ch, 0, numSamples));
    }

    currentRms /= static_cast<float>(numChannels);

    rms = currentRms;

    const auto currentTime = juce::Time::getMillisecondCounter();
    if (currentPeak >= peak || (currentTime - lastPeakTime) > holdTimeMs){
        peak = currentPeak;
        lastPeakTime = currentTime;
    }
}

float AudioMeterProcessor::getRMSLevel(const juce::dsp::AudioBlock<float>& block, int channel, int startSample, int numSamples){


    if (numSamples <= 0 || channel < 0 || channel >= (int)block.getNumChannels())
        return 0.0f;

    const auto* data = block.getChannelPointer(channel) + startSample;

    float sumOfSquares = 0.0f;
    for (int i = 0; i < numSamples; ++i){
        sumOfSquares += data[i] * data[i];
    }

    return std::sqrt(sumOfSquares / (float)(numSamples));
}

float AudioMeterProcessor::getMagnitude(const juce::dsp::AudioBlock<float>& block, int channel, int startSample, int numSamples){

    if (numSamples <= 0 || block.getNumChannels() == 0)
        return 0.0f;

    float maxMagnitude = 0.0f;
    
    const auto* data = block.getChannelPointer(channel) + startSample;
    
    for (int i = 0; i < numSamples; ++i){
        maxMagnitude = std::max(maxMagnitude, std::abs(data[i]));
    }

    return maxMagnitude;
}
