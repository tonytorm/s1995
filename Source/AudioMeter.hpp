//
//  AudioMeter.hpp
//  s1995
//
//  Created by giulio iacomino on 25/01/2025.
//

#ifndef AudioMeter_hpp
#define AudioMeter_hpp

#include <stdio.h>
#include <JuceHeader.h>
#include "AudioMeterProcessor.hpp"

class AudioMeter : public juce::Component, juce::Timer
{
public:
    AudioMeter(AudioMeterProcessor& ams);
    void paint(Graphics& g) override;
    void resized() override;
    void timerCallback() override;
    
    AudioMeterProcessor& source;
    
};

#endif /* AudioMeter_hpp */
