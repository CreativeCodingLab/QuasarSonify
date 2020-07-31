/*
  ==============================================================================

    This file was auto-generated!

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "GUI/MainView.h"
#include "AudioGraph/AudioGraph.h"
#include "Data/DataHandler.h"

//==============================================================================
/*
    This component lives inside our window, and this is where you should put all
    your controls and content.
*/
#define MAIN MainComponent::getInstance()


class MainComponent   : public AudioAppComponent
{
public:
    //==============================================================================

    //==============================================================================
    void prepareToPlay (int samplesPerBlockExpected, double sampleRate) override;
    void getNextAudioBlock (const AudioSourceChannelInfo& bufferToFill) override;
    void releaseResources() override;

    //==============================================================================
    void paint (Graphics& g) override;
    void resized() override;

    void init();
    static MainComponent* getInstance();
    
    
    
    ScopedPointer<MainView> mainView;
    
    ScopedPointer<AudioGraph> audioGraph;
    
    ScopedPointer<DataHandler> data;
    
    
private:
    
    MainComponent();
    ~MainComponent();
    
    
    MainComponent(MainComponent const&){}
    void operator=(MainComponent const&){}
    static MainComponent* instance;


    //==============================================================================
    // Your private member variables go here...


    //JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
};
