//
//  CustomSlider.hpp
//  s1995
//
//  Created by giulio iacomino on 23/01/2025.
//

#ifndef CustomSlider_hpp
#define CustomSlider_hpp

#include <stdio.h>
#include <JuceHeader.h>

class CustomSlider : public juce::Slider
{
public:
    CustomSlider();
    void paint(juce::Graphics& g) override;
    
    Colour sliderMainColour = {170, 170, 170};
};

#endif /* CustomSlider_hpp */
