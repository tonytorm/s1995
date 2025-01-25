//
//  s1995PluginAudioProcessorEditor.cpp
//  s1995
//
//  Created by giulio iacomino on 22/01/2025.
//

#include "s1995PluginAudioProcessorEditor.hpp"

s1995PluginAudioProcessorEditor::s1995PluginAudioProcessorEditor(s1995PluginAudioProcessor& p, juce::AudioProcessorValueTreeState& state) :
AudioProcessorEditor(&p),
audioProcessor(p),
parameters(state),
audioMeter(p.audioMeterSource)
{
    
    for (int s = 0; s < numSliders; ++s){
        sliders[s] = std::make_unique<CustomSlider>();
        addAndMakeVisible(*sliders[s]);
    }

    CustomSlider* gainSlider = sliders[0].get();
    
    gainSlider->sliderMainColour = {219, 92, 91};
    gainSlider->setRange(0.f, 1.f);
    gainSlider->setValue(0.1f);

    gainSlider->onValueChange = [this, gainSlider]{
        float normalized = gainSlider->getValue();
        parameters.getParameter("inputGain")->setValueNotifyingHost(normalized);
    };
    
    logoDrawable = loadLogoFromSVGData();
    setSize(500, 100);
    
    auto parameter = parameters.getParameter("cutofffrequency");
    cutoffAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(parameters, "cutofffrequency", *sliders[1]);
    sliders[1]->setRange(parameter->getNormalisableRange().start, parameter->getNormalisableRange().end);
    sliders[1]->setValue(parameter->getNormalisableRange().end);
    
    sliders[1]->onValueChange = [this]{
        std::cout << "cutoff: " << *parameters.getRawParameterValue("cutofffrequency") << '\n';
    };
    
    sliders[2]->setRange(0.f, 1.f);
    sliders[2]->setValue(0.4f);
    CustomSlider* outSlider = sliders[2].get();
    
    outSlider->onValueChange = [this, outSlider]{
        float normalized = outSlider->getValue();
        parameters.getParameter("outputGain")->setValueNotifyingHost(normalized);
    };
    
    addAndMakeVisible(audioMeter);
}

s1995PluginAudioProcessorEditor::~s1995PluginAudioProcessorEditor() {
    
}

void s1995PluginAudioProcessorEditor::paint(juce::Graphics& g){
    
    constexpr int textHeight = 20;
    constexpr int textY = 12;
    constexpr float fontHeight = 10.f;
    constexpr float sliderOutlineCornerRadius = 8.f;
    constexpr float pathStrokeWidth = 0.5f;
    constexpr float logoScale = 0.8f;
    constexpr float logoYOffset = -12.f;
    constexpr int pathExtraStroke = 2;

    // Background
    g.setColour(Colour{220, 220, 220});
    g.fillAll();

    // Slider labels
    g.setColour(Colour{98, 98, 98});
    g.setFont(Font(fontHeight, juce::Font::bold));

    for (int s = 0; s < numSliders; s++){
        auto textBounds = sliders[s]->getBounds()
                              .withHeight(textHeight)
                              .withY(textY);
        g.drawText(sliderNames[s], textBounds, Justification::centredTop);
    }

    // Slider area outline
    const int logoRight = logoDrawable->getWidth() + padding;
    const int slidersOutlineWidth = getWidth() - (padding * 3 + logoRight + meterWidth);
    const int sliderOutlineY = padding + 1;

    juce::Path slidersAreaOutline;
    slidersAreaOutline.addRoundedRectangle(
        logoRight + meterWidth + padding * 2,
        sliderOutlineY,
        slidersOutlineWidth,
        getHeight() - (padding) * 2 - pathExtraStroke,
        sliderOutlineCornerRadius,
        sliderOutlineCornerRadius,
        false, false, false, true
    );

    g.strokePath(slidersAreaOutline, PathStrokeType(1.0f, PathStrokeType::curved, PathStrokeType::rounded));

    // Logo
    logoDrawable->drawAt(
        g,
        static_cast<float>(padding),
        logoYOffset,
        logoScale
    );
}

void s1995PluginAudioProcessorEditor::resized(){
    
    logoArea = {0, 0, getWidth()/3, getHeight()};
    Rectangle<int> slidersArea = getBounds().removeFromRight(getWidth() - logoArea.getWidth());
    
    const int sliderMargin = 20;
    const int xOffset = slidersArea.getX();
    const int sliderWidth = slidersArea.getWidth()/numSliders;
    for(int s = 0; s < numSliders; s++){
        Rectangle<int> sliderBounds = {xOffset + (s * sliderWidth), sliderMargin/2, sliderWidth, getHeight()};
        sliders[s]->setBounds(sliderBounds.reduced(sliderMargin));
    }
    
    const int logoRight = logoDrawable->getWidth() + padding;
    audioMeter.setBounds(logoRight+padding, padding, meterWidth, getHeight() - padding*2);
    
}

std::unique_ptr<juce::Drawable> s1995PluginAudioProcessorEditor::loadLogoFromSVGData(){
    return juce::Drawable::createFromImageData(BinaryData::logo_svg, BinaryData::logo_svgSize);
}
