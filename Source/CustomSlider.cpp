//
//  CustomSlider.cpp
//  s1995
//
//  Created by giulio iacomino on 23/01/2025.
//

#include "CustomSlider.hpp"

CustomSlider::CustomSlider() : juce::Slider() {
    setTextBoxStyle(TextEntryBoxPosition::NoTextBox, true, 0, 0);
//    setSliderSnapsToMousePosition(true);
    setSliderStyle(Slider::Rotary);
}

void CustomSlider::paint(juce::Graphics& g) {
    constexpr float pi = juce::MathConstants<float>::pi;
    constexpr float ellipsePadding = 0.1f;
    constexpr float innerEllipsePadding = 0.25f;
    constexpr float dotRadiusFactor = 0.075f;
    constexpr int numDots = 14;
    constexpr float thumbWidth = 3.0f;
    constexpr float dotOffsetFactor = 0.5f;

    const float rotaryStartAngle = pi * 3.0f / 4.0f;
    const float rotaryEndAngle = pi * 9.0f / 4.0f;
    const float angleRange = rotaryEndAngle - rotaryStartAngle;
    
    const float range = getMaximum() - getMinimum();
    const float sliderPos = range != 0 ? (getValue() - getMinimum()) / range : 0.0f;
    const float sliderPosToAngle = rotaryStartAngle + sliderPos * angleRange;

    juce::Rectangle<float> bounds = getLocalBounds().toFloat();
    const float componentWidth = bounds.getWidth();
    bounds = bounds.removeFromBottom(componentWidth).withSizeKeepingCentre(componentWidth, componentWidth);
    
    g.setColour(sliderMainColour);
    g.fillEllipse(bounds.reduced(componentWidth * innerEllipsePadding));

    g.setColour(juce::Colours::darkgrey.withAlpha(0.3f));
    g.drawEllipse(bounds.reduced(componentWidth * innerEllipsePadding), 1.0f);
    
    g.setColour({207, 207, 207});
    g.fillEllipse(bounds.reduced(componentWidth * 0.3f));
                  
    g.setColour(juce::Colours::darkgrey.withAlpha(0.3f));
    g.drawEllipse(bounds.reduced(componentWidth * 0.3f), 1.f);

    g.setColour({227, 227, 227});
    g.fillEllipse(bounds.reduced(componentWidth * 0.35f));
    
    g.setColour({150, 150, 150});
    g.drawEllipse(bounds.reduced(componentWidth * 0.35f), 1.f);
    
    const float radius = componentWidth / 6.0f;
    const float lineW = juce::jmin(8.0f, radius * 0.5f);
    const float arcRadius = radius - lineW * 0.5f;

    juce::Point<float> thumbPoint(
        bounds.getCentreX() + arcRadius * std::cos(sliderPosToAngle),
        bounds.getCentreY() + arcRadius * std::sin(sliderPosToAngle)
    );

    const float rectWidth = radius;
    const float rectHeight = radius * 0.4f;
    juce::Rectangle<float> rectangle(rectWidth, rectHeight);
    
    const float angleToCenter = std::atan2(
        thumbPoint.y - bounds.getCentreY(),
        thumbPoint.x - bounds.getCentreX()
    );

    g.setColour(juce::Colours::darkgrey);
    
    const float offsetDistanceFromCentre = 2.f;
    thumbPoint.x += offsetDistanceFromCentre * std::cos(angleToCenter);
    thumbPoint.y += offsetDistanceFromCentre * std::sin(angleToCenter);
    
    rectangle.setCentre(thumbPoint);
    
    // Rotate the rectangle to point toward the center of the slider
    juce::AffineTransform transform = juce::AffineTransform::rotation(angleToCenter, thumbPoint.x, thumbPoint.y);
    Path p;
    p.addRoundedRectangle(rectangle, 3.f, 3.f);
    p.addRectangle(rectangle.withWidth(radius*0.6f));
    g.fillPath(p, transform);
    
}

