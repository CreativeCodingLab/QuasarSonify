/*
  ==============================================================================

    This file was auto-generated!

  ==============================================================================
*/

#include "MainComponent.h"

//==============================================================================

//initialize static member variables:
MainComponent* MainComponent::instance = 0;

//==============================================================================

MainComponent* MainComponent::getInstance()
{
    if (!instance)   // Only allow one instance of class to be generated.
        instance = new MainComponent();
    return instance;
}

void MainComponent::init()
{
    // initialize the view first
    mainView = new MainView;
    addAndMakeVisible(mainView);
    
    
    // if data takes a long time to load, show a status report...
//    String reportString;
//    reportString
//    << "Initializaing and loading data...." << newLine
//    << "Please wait about a minue..." << newLine;
//    MAIN->mainView->reportComponent.statusReport.setText(reportString);
    
    
    // load the data and point to it from the main view.
    data = new DataHandler;
    data->loadData(false);
    mainView->dataHandler = data;
    
    
    // initialize the audio and point to it from main view
    audioGraph = new AudioGraph(2);
    mainView->audioGraph = audioGraph;
    
    
    // initialize the state of the main view.
    mainView->setLoadedDataFileName(data->getLoadedFile().getFileName());
    mainView->setDefaultState();
    
    
    // Some platforms require permissions to open input channels so request that here
    if (RuntimePermissions::isRequired (RuntimePermissions::recordAudio)
        && ! RuntimePermissions::isGranted (RuntimePermissions::recordAudio))
    {
        RuntimePermissions::request (RuntimePermissions::recordAudio,
                                     [&] (bool granted) { if (granted)  setAudioChannels (2, 2); });
    }
    else
    {
        // Specify the number of input and output channels that we want to open
        setAudioChannels (2, 2);
    }
    
    setSize (700, 825);

}


MainComponent::MainComponent()
{
}


MainComponent::~MainComponent()
{
    // This shuts down the audio device and clears the audio source.
    shutdownAudio();
}

//==============================================================================
void MainComponent::prepareToPlay (int samplesPerBlockExpected, double sampleRate)
{
    // This function will be called when the audio device is started, or when
    // its settings (i.e. sample rate, block size, etc) are changed.

    // You can use this function to initialise any resources you might need,
    // but be careful - it will be called on the audio thread, not the GUI thread.

    // For more details, see the help for AudioProcessor::prepareToPlay()
}


void MainComponent::getNextAudioBlock (const AudioSourceChannelInfo& bufferToFill)
{
    // Right now we are not producing any data, in which case we need to clear the buffer
    // (to prevent the output of random noise)
    bufferToFill.clearActiveBufferRegion();
    
    if(audioGraph)
        audioGraph->processGraph(bufferToFill.buffer, bufferToFill.buffer->getNumChannels());
}


void MainComponent::releaseResources()
{
    // This will be called when the audio device stops, or when it is being
    // restarted due to a setting change.

    // For more details, see the help for AudioProcessor::releaseResources()
}

//==============================================================================
void MainComponent::paint (Graphics& g)
{
    
    
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));

    // You can add your drawing code here!
}

void MainComponent::resized()
{
    mainView->setBounds(getBounds());
    
    // This is called when the MainContentComponent is resized.
    // If you add any child components, this is where you should
    // update their positions.
}
