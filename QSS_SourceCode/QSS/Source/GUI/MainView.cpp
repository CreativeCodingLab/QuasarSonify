/*
  ==============================================================================

    This file was auto-generated!

  ==============================================================================
*/

#include "MainView.h"

//==============================================================================
MainView::MainView()
{
    // ------------ data configuration...
    
    dataConfigurationLabel.setText("Data Configuration:", dontSendNotification);
    dataConfigurationLabel.setEditable(false);
    addAndMakeVisible(dataConfigurationLabel);
    
    
    loadDataFileButton.reset(new TextButton);
    loadDataFileButton->setButtonText("Load Data File");
    loadDataFileButton->setColour(TextButton::buttonOnColourId, Colours::green);
    loadDataFileButton->addListener(this);
    addAndMakeVisible(loadDataFileButton.get());

    fileLoadedLabel.setText("Data File: None", dontSendNotification);
    fileLoadedLabel.setEditable(false);
    addAndMakeVisible(fileLoadedLabel);

    
    selectElementBox.reset(new ComboBox);
    selectElementBox->addListener(this);
    addAndMakeVisible(selectElementBox.get());
    
    selectElementLabel.setText("Selected Element:", dontSendNotification);
    selectElementLabel.setEditable(false);
    addAndMakeVisible(selectElementLabel);
    
    
    
    // ------------ audio controls...
    
    audioControlsLabel.setText("Audio Controls:", dontSendNotification);
    audioControlsLabel.setEditable(false);
    addAndMakeVisible(audioControlsLabel);
    
    playAudioButton.reset( new TextButton );
    playAudioButton->setClickingTogglesState(true);
    playAudioButton->setButtonText("Audio On/Off");
    playAudioButton->setColour(TextButton::buttonOnColourId, Colours::green);
    playAudioButton->addListener(this);
    addAndMakeVisible(*playAudioButton);
    
    mainGainSlider.reset( new Slider(Slider::RotaryVerticalDrag, Slider::NoTextBox) );
    mainGainSlider->setRange(0.0, 1.0);
    mainGainSlider->setSliderStyle(juce::Slider::RotaryVerticalDrag);
    mainGainSlider->addListener(this);
    addAndMakeVisible(*mainGainSlider);
    
    playbackComponents.clear();
    for(int i = 0; i < 3; i++)
    {
        playbackComponents.add(new SpectralPlaybackComponent);
        playbackComponents[i]->setButtonListeners(this);
        addChildComponent(playbackComponents[i]);
    }
    
    
    
    // ---------- data Navigation
    
    dataNavigationLabel.setText("Data Navigation:", dontSendNotification);
    dataNavigationLabel.setEditable(false);
    addAndMakeVisible(dataNavigationLabel);

    
    redShiftPositionSlider.setRange(0.01, 20);
    //grainRateSlider.setSliderStyle(juce::Slider::Rotary);
    redShiftPositionSlider.addListener(this);
    addAndMakeVisible(redShiftPositionSlider);
    redShiftPositionLabel.setText("Anchor Red Shift:", dontSendNotification);
    addAndMakeVisible(redShiftPositionLabel);

    
    
    velIncrementSlider.setRange(0.1, 25);
    velIncrementSlider.setSliderStyle(juce::Slider::LinearVertical);
    velIncrementSlider.addListener(this);
    velIncrementSlider.setTextBoxStyle(juce::Slider::TextEntryBoxPosition::TextBoxBelow, false, 50, 25);
    addAndMakeVisible(velIncrementSlider);
    velIncrementLabel.setText("Vel Incr", dontSendNotification);
    velIncrementLabel.attachToComponent(&velIncrementSlider, false);
    //grainRateSlider.setValue(1.0, sendNotification);
    addAndMakeVisible(velIncrementLabel);
    
    
    scrubKnob.setRange(-500000, 500000);
    scrubKnob.addListener(this);
    addAndMakeVisible(scrubKnob);
    
    velocityPositionLabel.setText("Velocity: ", dontSendNotification);
    velocityPositionLabel.setEditable(false);
    addAndMakeVisible(velocityPositionLabel);

    currentVelocity.setText("10", dontSendNotification);
    currentVelocity.setEditable(false);
    currentVelocity.setJustificationType(juce::Justification::left);
    addAndMakeVisible(currentVelocity);

    
    redShiftLabel.setText("Red Shift: ", dontSendNotification);
    redShiftLabel.setEditable(false);
    addAndMakeVisible(redShiftLabel);
    
    redShiftValueLabel.setText("10", dontSendNotification);
    redShiftValueLabel.setEditable(false);
    redShiftValueLabel.setJustificationType(juce::Justification::left);
    addAndMakeVisible(redShiftValueLabel);
    
    
    
    // plot the spectral lines
    spectralPlot.reset(new SpectrumPlotComponent);
    addAndMakeVisible(spectralPlot.get());
    
    
    
    processSpectralBatchButton.reset( new TextButton);
    processSpectralBatchButton->setButtonText("Process Batch");
    processSpectralBatchButton->setColour(TextButton::buttonOnColourId, Colours::green);
    processSpectralBatchButton->addListener(this);
    if(BATCH_SONIFY_TEST) addAndMakeVisible(processSpectralBatchButton.get());
    
    enableBatchPlaybackButton.reset( new TextButton);
    enableBatchPlaybackButton->setButtonText("Playback Batch");
    enableBatchPlaybackButton->setClickingTogglesState(true);
    enableBatchPlaybackButton->setColour(TextButton::buttonOnColourId, Colours::green);
    enableBatchPlaybackButton->addListener(this);
    if(BATCH_SONIFY_TEST) addAndMakeVisible(enableBatchPlaybackButton.get());

    
    

}

MainView::~MainView()
{
}


//==============================================================================
void MainView::paint (Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.setColour(Colours::darkgrey);
    g.fillAll();
    
    g.setColour(Colours::white);
    g.drawRoundedRectangle(dataConfigurationRect, 2.0, 1.0);
    
    g.drawRoundedRectangle(audioControlsRect, 2.0, 1.0);

    g.drawRoundedRectangle(dataNavigationRect, 2.0, 1.0);
}


void MainView::resized()
{
    int margin = 20;
    
    
    // data controls: load data and select element for analysis.
    dataConfigurationLabel.setBounds(margin, margin, 150, 25);
    dataConfigurationRect.setBounds(margin, dataConfigurationLabel.getBottom(), getWidth() - 2*margin, 110);
    
    loadDataFileButton->setBounds(margin + 10, dataConfigurationRect.getY() + 12, 150, 25);
    fileLoadedLabel.setBounds(loadDataFileButton->getRight()+10, loadDataFileButton->getY(), 300, 25);
    
    selectElementLabel.setBounds(loadDataFileButton->getX(), loadDataFileButton->getBottom() + 10, 150, 25);
    selectElementBox->setBounds(selectElementLabel.getX(), selectElementLabel.getBottom(), 150, 25);
    
    
    
    // main audio on/off and gain
    float buttonSize = 60;
    audioControlsLabel.setBounds(margin, dataConfigurationRect.getBottom() + 20, 150, 25);
    audioControlsRect.setBounds(margin, audioControlsLabel.getBottom(), 3*buttonSize, 85);
    
    playAudioButton->setBounds(margin + 10, audioControlsRect.getY() + 10, buttonSize, buttonSize);
    mainGainSlider->setBounds(playAudioButton->getRight() + 10, playAudioButton->getY(), buttonSize, buttonSize);
    
    
    
    int xPos = audioControlsRect.getRight() + 45;
    int yPos = dataConfigurationRect.getBottom() + 15;
    for(int i = 0; i < 3; i++)
    {
        playbackComponents[i]->setTopLeftPosition(xPos, yPos);
        xPos += playbackComponents[i]->getWidth();
    }
    
    
    
    // data navigation controls
    dataNavigationLabel.setBounds(margin, audioControlsRect.getBottom() + 30, 150, 25);
    dataNavigationRect.setBounds(margin, dataNavigationLabel.getBottom(), getWidth() - 2*margin, 300);
    
    // red shift slider
    redShiftPositionLabel.setBounds(margin + 10, dataNavigationRect.getY()+10, 200, 25);
    redShiftPositionSlider.setBounds(margin + 10, redShiftPositionLabel.getBottom(), dataNavigationRect.getWidth() - 30, 25);
    
    // fine detail slider and jog wheel
    float size = 150;
    scrubKnob.setBounds(getWidth()/2 - size/2, redShiftPositionSlider.getBottom()+50, size, size);
    velIncrementSlider.setBounds(scrubKnob.getX() - 75, scrubKnob.getY(), 75, scrubKnob.getHeight());

    
    // current velocity / red shift position labels
    velocityPositionLabel.setBounds(dataNavigationRect.getRight() - 150, dataNavigationRect.getBottom() - 60, 65, 25);
    currentVelocity.setBounds(velocityPositionLabel.getRight(), velocityPositionLabel.getY(), 100, 25);
    
    redShiftLabel.setBounds(velocityPositionLabel.getX(), velocityPositionLabel.getBottom(), 65, 25);
    redShiftValueLabel.setBounds(redShiftLabel.getRight(), redShiftLabel.getY(), 100, 25);
    
    
    spectralPlot->setBounds(margin, dataNavigationRect.getBottom() + 10, getWidth() - 2*margin, 150);
    spectralPlot->formatPlotAndRepaint();
    
        
    processSpectralBatchButton->setBounds(getRight() - 160, 10, 150, 25);
    enableBatchPlaybackButton->setBounds(getRight() - 160, processSpectralBatchButton->getBottom(), 150, 25);
}


void MainView::buttonClicked (Button* b)
{
    if(b == processSpectralBatchButton.get())
    {
        dataHandler->setBatchSpectralDataForSonification();
    }
    else if(b == enableBatchPlaybackButton.get())
    {
        //dataHandler->overviewPlayback->startStopPlaybackAll(enableBatchPlaybackButton->getToggleState());
        
        if(enableBatchPlaybackButton->getToggleState())
        {
            dataHandler->overviewPlayback->setFamilyPlaybackSpeed(eFamilyID_1, 2000);
            dataHandler->overviewPlayback->startFamilyPlayback(eFamilyID_1);
            
            dataHandler->overviewPlayback->setFamilyPlaybackSpeed(eFamilyID_4, 300);
            dataHandler->overviewPlayback->startFamilyPlayback(eFamilyID_4);
            
            dataHandler->overviewPlayback->setFamilyPlaybackSpeed(eFamilyID_5, 750);
            dataHandler->overviewPlayback->startFamilyPlayback(eFamilyID_5);
        }
        else
        {
            //dataHandler->overviewPlayback->stopFamilyPlayback(eFamilyID_1);
            
            dataHandler->overviewPlayback->startStopPlaybackAll(false); // stop playback on all families...
        }
        
    }
    else if(b == loadDataFileButton.get())
    {
        String loadedFileName = dataHandler->selectDataFileToLoad();
        
        if(loadedFileName != ""){
            fileLoadedLabel.setText("Data File: " + loadedFileName, dontSendNotification);
        }
    }
    else if(b == playAudioButton.get())
    {
        audioGraph->audioOn = b->getToggleState();
    }
    
    // check the spectral transition buttons...
    for(int i = 0; i < 3; i++)
    {
        if(b == playbackComponents[i]->enableTransition.get())
        {
            audioGraph->spectralElementProcessors[i].enableProcessing = b->getToggleState();
        }
        if(b == playbackComponents[i]->enableCentroid.get())
        {
            audioGraph->spectralElementProcessors[i].enableSkew = b->getToggleState();
            
            if(b->getToggleState())
            {
                audioGraph->spectralElementProcessors[i].enableACD = false;
                playbackComponents[i]->enableACD->setToggleState(false, dontSendNotification);
            }
        }
        else if(b == playbackComponents[i]->enableACD.get())
        {
            audioGraph->spectralElementProcessors[i].enableACD = b->getToggleState();
            
            if(b->getToggleState())
            {
                audioGraph->spectralElementProcessors[i].enableSkew = false;
                playbackComponents[i]->enableCentroid->setToggleState(false, dontSendNotification);
            }
        }
        if(b == playbackComponents[i]->widthButton.get())
        {
            if(i == 0)
            {
                audioGraph->triggerGliss_alpha();
            }
            else if(i == 1)
            {
                audioGraph->triggerGliss_beta();
            }
            else if(i == 2)
            {
                audioGraph->triggerGliss_gamma();
            }
        }
    }
}

void MainView::labelTextChanged (Label* l)
{
}

void MainView::sliderValueChanged (Slider* slider)
{
    if(slider == mainGainSlider.get())
    {
        float level = slider->getValue();
        audioGraph->setOutputLevel(level);
    }
    else if(slider == &velIncrementSlider)
    {
        float value = slider->getValue();
        
        scrubKnob.incrementValue = value;
    }
    else if(slider == &scrubKnob)
    {
        float velocityValue = scrubKnob.getValue();
        
        
        // compute the flux based on the input velocity with anchored red shift...
        dataHandler->getFluxFromVelocity(dataHandler->selectedSpectralData, velocityValue, currentRedShift);
        

        // compute the sonification parameters
        dataHandler->setSonificationSpectralAmplitudes(dataHandler->selectedSpectralData);
        dataHandler->setSonificationBandwidthParameters(dataHandler->selectedSpectralData);
        dataHandler->setACDFrequency(dataHandler->selectedSpectralData);
        
        
        //dataHandler->broadcastDataOSC(dataHandler->selectedSpectralData);
        
        
        // map the sonification parameters to audio
        audioGraph->mapSonificationParameters(dataHandler);
        
        
        // set velocity and red shift labels
        currentVelocity.setText(String(velocityValue), dontSendNotification);
        redShiftValueLabel.setText(String(dataHandler->redShiftatVelocity), dontSendNotification);
        
        
        // plot the absoprtion lines
        int indx1 = dataHandler->selectedSpectralData.size() > 0 ? dataHandler->selectedSpectralData[0].rawDataIndex : -1;
        int indx2 = dataHandler->selectedSpectralData.size() > 1 ? dataHandler->selectedSpectralData[1].rawDataIndex : -1;
        int indx3 = dataHandler->selectedSpectralData.size() > 2 ? dataHandler->selectedSpectralData[2].rawDataIndex : -1;
        spectralPlot->plotCenterAndWidth(indx1, indx2, indx3, 100);
    }
    else if(slider = &redShiftPositionSlider)
    {
        currentRedShift = slider->getValue();
        
        scrubKnob.setValue(.01, sendNotification); //NOTE: need to set two different values to ensure a different value is set and the slider callback gets triggered.
        scrubKnob.setValue(0, sendNotification);
    }
    
}

void MainView::comboBoxChanged (ComboBox* cb)
{
    if(cb = selectElementBox.get())
    {
        int selectedIndx = selectElementBox->getSelectedId() - 1;
        dataHandler->setSpectralDataForAnalysis(selectedIndx);
        
        
        
        // ----- reset the data calculations...
        // compute the flux based on the input velocity...
        //dataHandler->getFluxFromRedShift(dataHandler->selectedSpectralData, currentRedShift);
        dataHandler->getFluxFromVelocity(dataHandler->selectedSpectralData, 0, currentRedShift);

        
        // compute the sonification parameters
        dataHandler->setSonificationSpectralAmplitudes(dataHandler->selectedSpectralData);
        dataHandler->setSonificationBandwidthParameters(dataHandler->selectedSpectralData);
        dataHandler->setACDFrequency(dataHandler->selectedSpectralData);
        
        //dataHandler->broadcastDataOSC(dataHandler->selectedSpectralData);
        
        // map the sonification parameters to audio
        audioGraph->mapSonificationParameters(dataHandler);
        //audioGraph->setSonificationParameters(dataHandler);

        
        // reset the slider and jog wheel ranges
        redShiftPositionSlider.setRange(dataHandler->minMaxRedShift_Absolute[0], dataHandler->minMaxRedShift_Absolute[1]);
        scrubKnob.setRange(dataHandler->minMaxVelocity_Absolute[0], dataHandler->minMaxVelocity_Absolute[1]);

        
        // ----- update the view...
        for(int i = 0; i < 3; i++)
        {
            if(i < dataHandler->selectedSpectralData.size())
            {
                playbackComponents[i]->setVisible(true);
                playbackComponents[i]->enableTransition->setToggleState(true, sendNotification);
                playbackComponents[i]->specTransitionLabel.setText(dataHandler->selectedSpectralData[i].displayName, dontSendNotification);
            }
            else
            {
                playbackComponents[i]->enableTransition->setToggleState(false, sendNotification);
                playbackComponents[i]->setVisible(false);
            }
        }
    }
}


// NOTE: set the default state of the view to sync with the data and audio.
// Run this routine AFTER the data has been initialized and aduio has been initialized.
void MainView::setDefaultState()
{
    // populate the plot data
    spectralPlot->setSpectralPlotData(dataHandler->rawWavelengthData);
    
    
    // load the atom properties list
    dataHandler->loadAtomicDataList();
    
    // populate the atom selection box
    selectElementBox->clear();
    for(int i = 0; i < dataHandler->atomSpectraList.size(); i++)
    {
        selectElementBox->addItem(dataHandler->atomSpectraList[i][0].label, i+1);
    }
    
    // select hydrogen as the default element to analyze (index = 2 in the atomic list)
    int hydrogenIndx = 2;
    dataHandler->setSpectralDataForAnalysis(hydrogenIndx - 1);
    selectElementBox->setSelectedId(hydrogenIndx, sendNotification);

    
    redShiftPositionSlider.setRange(dataHandler->minMaxRedShift_Absolute[0], dataHandler->minMaxRedShift_Absolute[1]);
    scrubKnob.setRange(dataHandler->minMaxVelocity_Absolute[0], dataHandler->minMaxVelocity_Absolute[1]);
    
    
    redShiftPositionSlider.setValue(0.20257);
    velIncrementSlider.setValue(0.5);
}

