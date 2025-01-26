//
//  s1995PluginAudioProcessor.cpp
//  s1995
//
//  Created by giulio iacomino on 22/01/2025.
//

#include "s1995PluginAudioProcessor.hpp"
#include "s1995PluginAudioProcessorEditor.hpp"

using namespace juce;

// from (0-1) to db
static float convertGainFrom0To1InGain(float start, float end, float normalized) {
    constexpr float normalizedZeroPosition = 0.1f;
    const float negStart = -start;
    
    if (normalized < normalizedZeroPosition) {
        float rangeBelowZero = normalizedZeroPosition;
        return -negStart + negStart * std::pow(normalized / rangeBelowZero, 1.f/2.0f);
    } else {
        float rangeAboveZero = 1.0f - normalizedZeroPosition;
        return 0.0f + end * (normalized - normalizedZeroPosition) / rangeAboveZero;
    }
}

// from dB to (0-1)
static float convertGainTo0To1InGain(float start, float end, float db) {
    constexpr float normalizedZeroPosition = 0.1f;
    const float negStart = -start;
    
    if (db < 0.0f) {
        float t = (db + negStart) / negStart;
        return normalizedZeroPosition * std::pow(t, 2.0f);
    } else {
        float rangeAboveZero = 1.0f - normalizedZeroPosition;
        return normalizedZeroPosition + (db / end) * rangeAboveZero;
    }
}

// from (0-1) to db
static float convertGainFrom0To1OutGain(float start, float end, float normalized) {
    constexpr float normalizedZeroPosition = 0.4f;
    const float negStart = -start;
    
    if (normalized < normalizedZeroPosition) {
        float rangeBelowZero = normalizedZeroPosition;
        return -negStart + negStart * std::pow(normalized / rangeBelowZero, 1.f/3.0f);
    } else {
        float rangeAboveZero = 1.0f - normalizedZeroPosition;
        return 0.0f + end * (normalized - normalizedZeroPosition) / rangeAboveZero;
    }
}

// from dB to (0-1)
static float convertGainTo0To1OutGain(float start, float end, float db) {
    constexpr float normalizedZeroPosition = 0.4f;
    const float negStart = -start;
    
    if (db < 0.0f) {
        float t = (db + negStart) / negStart;
        return normalizedZeroPosition * std::pow(t, 3.0f);
    } else {
        float rangeAboveZero = 1.0f - normalizedZeroPosition;
        return normalizedZeroPosition + (db / 9.0f) * rangeAboveZero;
    }
}

s1995PluginAudioProcessor::s1995PluginAudioProcessor() :
AudioProcessor(BusesProperties()
               .withInput("Input", AudioChannelSet::stereo(), true)
               .withOutput("Output", AudioChannelSet::stereo(), true)),
parameters(*this, nullptr, "PARAMETERS",
           {std::make_unique<juce::AudioParameterFloat>("inputGain","InputGain", juce::NormalisableRange<float>(-60.f, 20.f, convertGainFrom0To1InGain, convertGainTo0To1InGain), 0.f),
            std::make_unique<AudioParameterFloat>("cutofffrequency", "CutoffFrequency", 100.f, 15000.f, 15000.f),
            std::make_unique<AudioParameterFloat>("outputGain", "OutputGain", juce::NormalisableRange<float>(-60.f, 9.f, convertGainFrom0To1OutGain, convertGainTo0To1OutGain), 0.f)}),
preadcChain(parameters)
{
}

s1995PluginAudioProcessor::~s1995PluginAudioProcessor() {
    
}

void s1995PluginAudioProcessor::prepareToPlay(double sampleRate, int samplesPerBlock) {
    juce::dsp::ProcessSpec spec;
    spec.maximumBlockSize = samplesPerBlock;
    spec.sampleRate = sampleRate;
    spec.numChannels = getTotalNumInputChannels();
    
    preadcChain.prepare(spec);
    adcChain.prepare(spec);
    dacChain.prepare(spec);
    lofilter.prepare(spec);
    lofilter.setCutoffFrequency(*parameters.getRawParameterValue("cutofffrequency"));
    audioMeterSource.reset();
}



void s1995PluginAudioProcessor::processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer&) {
    juce::dsp::AudioBlock<float> inputBlock(buffer);
    
    preadcChain.process(inputBlock);
    adcChain.processBlock(inputBlock);
    
    float cutoffFrequency = *parameters.getRawParameterValue("cutofffrequency");
    lofilter.setCutoffFrequency(cutoffFrequency);
    audioMeterSource.processBlock(inputBlock);
    lofilter.process(inputBlock);
    
    float outputGainValue = *parameters.getRawParameterValue("outputGain");
    dacChain.setGain(outputGainValue);
    dacChain.process(inputBlock);
}

void s1995PluginAudioProcessor::releaseResources(){
}

void s1995PluginAudioProcessor::getStateInformation(juce::MemoryBlock& destData){
    auto state = parameters.copyState();
    std::unique_ptr<juce::XmlElement> xml(state.createXml());
    copyXmlToBinary(*xml, destData);
}

void s1995PluginAudioProcessor::setStateInformation(const void* data, int sizeInBytes){
    std::unique_ptr<juce::XmlElement> xmlState (getXmlFromBinary (data, sizeInBytes));
    
    if (xmlState.get() != nullptr)
        if (xmlState->hasTagName (parameters.state.getType()))
            parameters.replaceState (juce::ValueTree::fromXml (*xmlState));

}


juce::AudioProcessorEditor* s1995PluginAudioProcessor::createEditor() { return new s1995PluginAudioProcessorEditor(*this, parameters); }
bool s1995PluginAudioProcessor::hasEditor() const { return true; }
const juce::String s1995PluginAudioProcessor::getName() const { return "s1995"; }
bool s1995PluginAudioProcessor::acceptsMidi() const { return false; }
bool s1995PluginAudioProcessor::producesMidi() const { return false; }
bool s1995PluginAudioProcessor::isMidiEffect() const { return false; }
double s1995PluginAudioProcessor::getTailLengthSeconds() const { return 0.0; }
int s1995PluginAudioProcessor::getNumPrograms() { return 1; }
int s1995PluginAudioProcessor::getCurrentProgram() { return 0; }
void s1995PluginAudioProcessor::setCurrentProgram(int) {}
const juce::String s1995PluginAudioProcessor::getProgramName(int) { return {}; }
void s1995PluginAudioProcessor::changeProgramName(int, const juce::String&) {}
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter() { return new s1995PluginAudioProcessor(); }



