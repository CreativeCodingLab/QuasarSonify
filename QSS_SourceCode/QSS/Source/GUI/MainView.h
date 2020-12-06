/*
  ==============================================================================

    This file was auto-generated!

  ==============================================================================
*/

#pragma once

#include "../../JuceLibraryCode/JuceHeader.h"
#include "../AudioGraph/AudioGraph.h"
#include "../GlobalDefines.h"
#include "../Data/DataHandler.h"
#include "DataReportComponent.h"
#include "CustomKnob.h"
#include "SpectralPlaybackComponent.h"
#include "SpectrumPlotComponent.h"

//==============================================================================
/*
    This component lives inside our window, and this is where you should put all
    your controls and content.
*/


class MainView  : public Component, public Slider::Listener, public Button::Listener, public Label::Listener, public ComboBox::Listener
{
public:
    //==============================================================================
    MainView();
    ~MainView();

    virtual void buttonClicked (Button* b) override;
    
    virtual void labelTextChanged (Label* labelThatHasChanged) override;
    
    virtual void sliderValueChanged (Slider* slider) override;
    
    virtual void comboBoxChanged (ComboBox* cb) override;


    void paint (Graphics& g) override;
    
    void resized() override;
    
    void setDefaultState();
        
    void setLoadedDataFileName(String fileName)
    {
        fileLoadedLabel.setText("Data File: " + fileName, dontSendNotification);
    }

    
    
    AudioGraph *audioGraph;
    DataHandler *dataHandler;
    
    
    // data configuration
    Label dataConfigurationLabel;
    Rectangle<float> dataConfigurationRect;
    
    std::unique_ptr<TextButton> loadDataFileButton;
    Label fileLoadedLabel;
    
    std::unique_ptr<ComboBox> selectElementBox;
    Label selectElementLabel;
    
    
    
    // audio controls
    Label audioControlsLabel;
    Rectangle<float> audioControlsRect;
    
    std::unique_ptr<TextButton> playAudioButton;
    
    std::unique_ptr<Slider> mainGainSlider;
    Label   gainSliderLabel;

    // spectral transition playback controls
    OwnedArray<SpectralPlaybackComponent> playbackComponents;
    
    
    
    // data navigation controls
    Label dataNavigationLabel;
    Rectangle<float> dataNavigationRect;

    Slider redShiftPositionSlider;
    Label redShiftPositionLabel;
    
    Slider  velIncrementSlider;
    Label   velIncrementLabel;
    
    CustomKnob scrubKnob;
    
    Label velocityPositionLabel;
    Label currentVelocity;
    
    Label redShiftLabel;
    Label redShiftValueLabel;

    
    
    // Line Plots
    std::unique_ptr<SpectrumPlotComponent> spectralPlot;
    
    
    // process batch sonification button
    std::unique_ptr<TextButton> processSpectralBatchButton;
    std::unique_ptr<TextButton> enableBatchPlaybackButton;
    
    
private:
    //==============================================================================
    // Your private member variables go here...
    
    
    float currentRedShift{0};


    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainView)
};


