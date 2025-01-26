//
//  s1995PluginAudioProcessor.hpp
//  s1995
//
//  Created by giulio iacomino on 22/01/2025.
//

#pragma once

#include <JuceHeader.h>
#include "PreADCChain.hpp"
#include "ADCChain.hpp"
#include "DACChain.hpp"
#include "AudioMeterProcessor.hpp"
#include "LoFilter.hpp"

class s1995PluginAudioProcessor : public juce::AudioProcessor
{
public:
    s1995PluginAudioProcessor();
    ~s1995PluginAudioProcessor() override;

    void prepareToPlay(double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;
    void processBlock(juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    const juce::String getName() const override;
    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram(int index) override;
    const juce::String getProgramName(int index) override;
    void changeProgramName(int index, const juce::String& newName) override;

    void getStateInformation(juce::MemoryBlock& destData) override;
    void setStateInformation(const void* data, int sizeInBytes) override;

    juce::AudioProcessorValueTreeState parameters;
    AudioMeterProcessor audioMeterSource;
    
    std::array<std::string, 3> parameterIDs {"inputGain", "cutofffrequency", "outputGain"};
private:

    PreADCChain preadcChain;
    ADCChain adcChain;
    DACChain dacChain;
    LoFilter lofilter;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(s1995PluginAudioProcessor)
};
