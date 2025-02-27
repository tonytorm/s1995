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
        auto& slider = sliders[s];
        slider = std::make_unique<CustomSlider>();
        slider->setName(sliderNames[s]);
        slider->setTitle(audioProcessor.parameterIDs[s]);
        slider->addMouseListener(this, false);
        addAndMakeVisible(*slider);
        
        if (s == 0){ // Gain Slider
        
            auto parameter = parameters.getParameter("inputGain");
            slider->sliderMainColour = {219, 92, 91};
            slider->setRange(0.f, 1.f);
            slider->setValue(parameter->getValue());

            slider->onValueChange = [this, &slider, parameter] {
                float normalized = slider->getValue();
                parameter->setValueNotifyingHost(normalized);
            };
            
        }else if (s == 1){ // Cutoff Slider
        
            auto* parameter = parameters.getParameter("cutofffrequency");
            slider->setRange(100.f, 15000.f);
            slider->setValue(parameter->getValue());
            cutoffAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(parameters, "cutofffrequency", *slider);
        }else if (s == 2){ // Output Slider
        
            auto parameter = parameters.getParameter("outputGain");
            slider->setRange(0.f, 1.f);
            slider->setValue(parameter->getValue());
            
            slider->onValueChange = [this, &slider, parameter] {
                float normalized = slider->getValue();
                parameter->setValueNotifyingHost(normalized);
            };
        }
    }
    
    logoDrawable = loadLogoFromSVGData();
    addAndMakeVisible(audioMeter);
    setSize(500, 100);
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
    
    if (hoveredSliderIndex != -1){
        Rectangle<int> sliderComponentBounds = sliders[hoveredSliderIndex]->getBounds(); // outer bounds
        Rectangle<int> knobBounds = sliderComponentBounds.reduced(sliderComponentBounds.getWidth() * 0.25f);
        int radius = knobBounds.getWidth()/2;
        
        const int tooltipBoxWidth = 48;
        const int tooltipBoxHeight = 20;
        const int tooltipBoxX = hoveredSliderIndex == 2 ? knobBounds.getX()-tooltipBoxWidth : knobBounds.getRight();
        const int tooltipBoxY = knobBounds.getCentreY() - tooltipBoxHeight/2;
        
        Rectangle<int>tooltipBox = {tooltipBoxX, tooltipBoxY, tooltipBoxWidth, tooltipBoxHeight};
        
        g.setColour(juce::Colours::black.withAlpha(0.7f));
        g.fillRoundedRectangle(tooltipBox.toFloat(), 4.0f);
        
        g.setColour(juce::Colours::white);
        g.setFont(juce::Font(11.0f, juce::Font::bold));
        g.drawText(currentTooltipValue + tooltipSuffix[hoveredSliderIndex], tooltipBox, juce::Justification::centred);
    }
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

void s1995PluginAudioProcessorEditor::mouseEnter(const juce::MouseEvent& event){
    for (int i = 0; i < numSliders; ++i){
        if (event.eventComponent == sliders[i].get()){
            hoveredSliderIndex = i;
            updateCurrentTooltipValue();
            break;
        }
    }
}

void s1995PluginAudioProcessorEditor::mouseExit(const juce::MouseEvent& event){
    if (hoveredSliderIndex != -1 && event.eventComponent == sliders[hoveredSliderIndex].get()){
        hoveredSliderIndex = -1;
        repaint();
    }
}

void s1995PluginAudioProcessorEditor::mouseDown(const juce::MouseEvent &event){
    if (hoveredSliderIndex != -1){
        updateCurrentTooltipValue();
    }
}

void s1995PluginAudioProcessorEditor::mouseDrag(const juce::MouseEvent &event){
    if (hoveredSliderIndex != -1){
        updateCurrentTooltipValue();
    }
}

void s1995PluginAudioProcessorEditor::updateCurrentTooltipValue(){
    auto parameter = parameters.getParameter(sliders[hoveredSliderIndex]->getTitle());
    if(hoveredSliderIndex==1){
        currentTooltipValue = juce::String((int)jmap((float)*parameters.getRawParameterValue(parameter->paramID), 100.f, 15000.f, 1.f, 99.f));
    }else{
        currentTooltipValue = juce::String(*parameters.getRawParameterValue(parameter->paramID), 1);
    }
    repaint();
}
