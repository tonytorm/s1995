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
    
    void mouseEnter(const juce::MouseEvent& event) override;
    void mouseExit(const juce::MouseEvent& event) override;
    void mouseDown(const juce::MouseEvent& event) override;
    void mouseDrag(const juce::MouseEvent& event) override;
    
    void updateHoveredSlider(CustomSlider* slider);
    
    std::unique_ptr<juce::Drawable> loadLogoFromSVGData();

private:
    const int numSliders = 3;
    const int padding = 4;
    const int meterWidth = 60;

    std::array<std::unique_ptr<CustomSlider>, 3> sliders;
    std::array<std::string, 3> sliderNames {"INPUT GAIN", "LOPASS", "OUTPUT LEVEL"};
    std::array<std::string, 3> tooltipSuffix {" dB", "", " dB"};
    Rectangle<int> logoArea;
    AudioMeter audioMeter;
    std::unique_ptr<juce::Drawable> logoDrawable;
    int hoveredSliderIndex = -1;
    juce::String currentTooltipValue = "";
    juce::Rectangle<int> tooltipBox;

    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> cutoffAttachment;
    
        
    s1995PluginAudioProcessor& audioProcessor;
    juce::AudioProcessorValueTreeState& parameters;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(s1995PluginAudioProcessorEditor)
};
