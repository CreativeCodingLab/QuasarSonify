/*
  ==============================================================================

    This file was auto-generated!

  ==============================================================================
*/

#pragma once

#include "../../JuceLibraryCode/JuceHeader.h"
#include "../AudioGraph/AudioGraph.h"
#include "../GlobalDefines.h"

//==============================================================================
/*
    This component lives inside our window, and this is where you should put all
    your controls and content.
*/



class SpectralPlaybackComponent : public Component
{
public:
    
    SpectralPlaybackComponent();
    ~SpectralPlaybackComponent(){}
    
    void paint(Graphics &g) override;
    void resized() override;
    
    
    void setButtonListeners(juce::Button::Listener *bListen)
    {
        enableTransition->addListener(bListen);
        enableCentroid->addListener(bListen);
        enableACD->addListener(bListen);
        widthButton->addListener(bListen);
    }
    
    
    Label specTransitionLabel;
    
    Label enableDroneLabel;
    std::unique_ptr<ToggleButton> enableTransition;
    
    Label enableCentroidLabel;
    std::unique_ptr<ToggleButton> enableCentroid;
    
    Label enableACDLabel;
    std::unique_ptr<ToggleButton> enableACD;

    
    Label triggerWidthLabel;
    std::unique_ptr<TextButton> widthButton;

    
    
    String spectralTransitionName{""};
    
private:
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SpectralPlaybackComponent)
};




