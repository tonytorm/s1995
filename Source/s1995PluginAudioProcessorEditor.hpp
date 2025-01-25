//
//  s1995PluginAudioProcessorEditor.hpp
//  s1995
//
//  Created by giulio iacomino on 22/01/2025.
//

#pragma once

#include <JuceHeader.h>
#include "s1995PluginAudioProcessor.hpp"
#include "CustomSlider.hpp"
#include "AudioMeter.hpp"

class s1995PluginAudioProcessorEditor : public juce::AudioProcessorEditor
{
public:
    s1995PluginAudioProcessorEditor(s1995PluginAudioProcessor&, juce::AudioProcessorValueTreeState& state);
    ~s1995PluginAudioProcessorEditor() override;

    void paint(juce::Graphics&) override;
    void resized() override;
    
    std::unique_ptr<juce::Drawable> loadLogoFromSVGData();

private:
    const int numSliders = 3;
    const int padding = 4;
    const int meterWidth = 60;

    std::array<std::unique_ptr<CustomSlider>, 3> sliders;
    std::array<std::string, 3> sliderNames {"INPUT GAIN", "LOPASS", "OUTPUT LEVEL"};
    Rectangle<int> logoArea;
    AudioMeter audioMeter;
    std::unique_ptr<juce::Drawable> logoDrawable;

    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> cutoffAttachment;

    
        
    s1995PluginAudioProcessor& audioProcessor;
    juce::AudioProcessorValueTreeState& parameters;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(s1995PluginAudioProcessorEditor)
};
