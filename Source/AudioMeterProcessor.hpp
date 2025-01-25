//
//  AudioMeterSource.hpp
//  s1995
//
//  Created by giulio iacomino on 25/01/2025.
//

#ifndef AudioMeterSource_hpp
#define AudioMeterSource_hpp

#include <stdio.h>
#include <atomic>
#include <JuceHeader.h>



class AudioMeterProcessor
{
public:
    AudioMeterProcessor(float holdTimeMs = 1000.0f): holdTimeMs(holdTimeMs), lastPeakTime(0), peak(0.0f), rms(0.0f){ }

    void reset();
    void processBlock(const juce::dsp::AudioBlock<float> block);
    float getRMSLevel(const juce::dsp::AudioBlock<float>& block, int channel, int startSample, int numSamples);
    float getMagnitude(const juce::dsp::AudioBlock<float>& block, int channel, int startSample, int numSamples);


    float holdTimeMs;
    uint32_t lastPeakTime;
    float peak;
    float rms;                     
};

#endif /* AudioMeterSource_hpp */
