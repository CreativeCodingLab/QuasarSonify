

#pragma once

#include "../../JuceLibraryCode/JuceHeader.h"


class CustomKnob : public Slider
{
public:
    CustomKnob();
    ~CustomKnob();
    
    void paint (Graphics& g) override;
    
    void mouseDown(const MouseEvent&) override;
    void mouseUp(const MouseEvent&) override;
    void mouseDrag(const MouseEvent&) override;

    float incrementValue{1};
    
private:
	
    float priorMouseX{0};
    float currentPosition{0};
	
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (CustomKnob)
};
