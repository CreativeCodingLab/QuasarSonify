/*
  ==============================================================================

    This file was auto-generated!

  ==============================================================================
*/

#include "SpectralPlaybackComponent.h"

//==============================================================================


SpectralPlaybackComponent::SpectralPlaybackComponent()
{
    

    specTransitionLabel.setText("SPEC TRANS", dontSendNotification);
    specTransitionLabel.setJustificationType(juce::Justification::centred);
    addAndMakeVisible(&specTransitionLabel);
    
    
    enableTransition.reset( new ToggleButton );
    addAndMakeVisible(enableTransition.get());
    enableDroneLabel.attachToComponent(enableTransition.get(), true);
    enableDroneLabel.setText("Presence:", dontSendNotification);
    enableDroneLabel.setJustificationType(juce::Justification::left);
    addAndMakeVisible(enableDroneLabel);

    
    enableCentroid.reset( new ToggleButton );
    addAndMakeVisible(enableCentroid.get());
    enableCentroidLabel.attachToComponent(enableCentroid.get(), true);
    enableCentroidLabel.setText("Centroid:", dontSendNotification);
    enableCentroidLabel.setJustificationType(juce::Justification::left);
    addAndMakeVisible(enableCentroidLabel);
    
    
    enableACD.reset( new ToggleButton );
    addAndMakeVisible(enableACD.get());
    enableACDLabel.attachToComponent(enableACD.get(), true);
    enableACDLabel.setText("ACD:", dontSendNotification);
    enableACDLabel.setJustificationType(juce::Justification::left);
    addAndMakeVisible(enableACDLabel);


    widthButton.reset( new TextButton("") );
    //widthButton->setColour(TextButton::buttonOnColourId, Colours::blue);
    addAndMakeVisible(widthButton.get());
    triggerWidthLabel.attachToComponent(widthButton.get(), true);
    triggerWidthLabel.setText("Width:", dontSendNotification);
    triggerWidthLabel.setJustificationType(juce::Justification::left);
    addAndMakeVisible(triggerWidthLabel);

    
    setSize(125, 150);
}

void SpectralPlaybackComponent::paint(Graphics& g)
{
    g.setColour(Colours::black.withAlpha(0.25f));
    g.fillAll();
    
    g.setColour(Colours::white);
    g.drawRoundedRectangle(getLocalBounds().toFloat(), 2.0, 1);
    
    g.drawHorizontalLine(specTransitionLabel.getBottom(), 0, getWidth());
}

void SpectralPlaybackComponent::resized()
{
    float margin = 10;
    specTransitionLabel.setBounds(0, 0, getWidth(), 25);
    
    
    float buttonSize = 25;
    float shift = 80;
    enableTransition->setBounds(margin + shift, specTransitionLabel.getBottom()+10, buttonSize, buttonSize);
    
    enableCentroid->setBounds(margin + shift, enableTransition->getBottom(), buttonSize, buttonSize);
    
    enableACD->setBounds(margin + shift, enableCentroid->getBottom(), buttonSize, buttonSize);
    
    buttonSize = 23;
    widthButton->setBounds(margin + shift, enableACD->getBottom() + 5, buttonSize, buttonSize);

}


