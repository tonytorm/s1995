//
//  AudioMeter.cpp
//  s1995
//
//  Created by giulio iacomino on 25/01/2025.
//

#include "AudioMeter.hpp"

AudioMeter::AudioMeter(AudioMeterProcessor& ams): source(ams) {
    startTimerHz(30);
}
void AudioMeter::paint(Graphics& g){
    const int numBars = 10;
    const float rms = source.rms;
    const int activeBars = std::ceil(rms * numBars);
    
    const auto bounds = getLocalBounds();
    const int barWidth = bounds.getWidth() / numBars;
    const int barHeight = bounds.getHeight();
    
    for (int i = 0; i < numBars; ++i){
        
        if (i < activeBars){
            if (i < numBars * 0.5f)
                g.setColour(Colours::green.brighter(0.5f));
            else if (i < numBars * 0.8f)
                g.setColour(Colours::yellow.darker(0.3f));
            else
                g.setColour(Colours::red.darker(0.3f));
        }else{
            g.setColour(Colours::darkgrey);
        }
        
        g.fillRect(i * barWidth, 0, barWidth - 2, barHeight);
    }
}
void AudioMeter::resized(){
}
void AudioMeter::timerCallback(){
    repaint();
}

