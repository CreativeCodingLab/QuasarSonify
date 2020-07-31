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



class StatusReportViewport : public Viewport
{
public:
    StatusReportViewport();
    ~StatusReportViewport(){}
    
    void paint(Graphics &g) override;
    void resized() override;
    
    void setStatusReportText(String text);
    
    TextEditor statusReport;
    
private:
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (StatusReportViewport)
};

class DataReportComponent  : public Component, public Button::Listener
{
public:
    //==============================================================================
    DataReportComponent();
    ~DataReportComponent();

    virtual void buttonClicked (Button* b) override;
    

    //==============================================================================
    void paint (Graphics& g) override;
    void resized() override;
    
    
    //StatusReportViewport statusReportViewport;
    TextEditor statusReport;
    
    TextButton clearOutputButton;
    
    
private:
    //==============================================================================
    // Your private member variables go here...


    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DataReportComponent)
};




