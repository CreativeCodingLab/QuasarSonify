/*
  ==============================================================================

    This file was auto-generated!

  ==============================================================================
*/

#include "DataReportComponent.h"

//==============================================================================


StatusReportViewport::StatusReportViewport()
{
    setScrollBarsShown(true, false);
    getVerticalScrollBar().setColour(ScrollBar::thumbColourId, Colours::grey.withAlpha(0.35f));
    
    
    statusReport.setReadOnly (true);
    statusReport.setMultiLine (true);
    //editor->setColour (TextEditor::backgroundColourId, Colours::transparentBlack);
    statusReport.setColour (TextEditor::backgroundColourId, Colours::lightgrey);
    statusReport.setColour (TextEditor::textColourId, Colours::black);
    statusReport.setFont (Font (Font::getDefaultMonospacedFontName(), 12.0f, Font::plain));
    statusReport.setSize(getWidth(), statusReport.getHeight());
    setViewedComponent(&statusReport);
    
    
    String reportString;
    reportString
    << "Display " << "xxxxxx" << ":" << newLine
    << "  Display Name:  " << "xxxxxx" << newLine
    << "  Is Raven Montior:  " << "xxxxxx"  << newLine
    << newLine;

    statusReport.setText(reportString);
    
    setSize(500, 200);
}

void StatusReportViewport::paint(Graphics& g)
{
    g.setColour(Colours::green);
    g.fillAll();
}

void StatusReportViewport::resized()
{
    int width = getWidth() - getScrollBarThickness();
    statusReport.setSize(width, 1000);
}

void StatusReportViewport::setStatusReportText(String text)
{
    statusReport.setText(text);
}


//==============================================================================
//==============================================================================


DataReportComponent::DataReportComponent()
{
    //addAndMakeVisible(statusReportViewport);
    
    statusReport.setReadOnly (true);
    statusReport.setMultiLine (true);
    //editor->setColour (TextEditor::backgroundColourId, Colours::transparentBlack);
    statusReport.setColour (TextEditor::backgroundColourId, Colours::lightgrey);
    statusReport.setColour (TextEditor::textColourId, Colours::black);
    statusReport.setFont (Font (Font::getDefaultMonospacedFontName(), 12.0f, Font::plain));
    //statusReport.setSize(getWidth(), statusReport.getHeight());
    addAndMakeVisible(statusReport);
    
    clearOutputButton.setClickingTogglesState(false);
    clearOutputButton.setButtonText("Clear Report");
    clearOutputButton.setColour(TextButton::buttonOnColourId, Colours::green);
    clearOutputButton.addListener(this);
    //addAndMakeVisible(clearOutputButton);
    
    setSize(100, 300);
}

DataReportComponent::~DataReportComponent()
{
}

void DataReportComponent::paint (Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.setColour(Colours::yellow);
    g.fillAll();

    // You can add your drawing code here!
}

void DataReportComponent::resized()
{
    statusReport.setBounds(10, 10, getWidth() - 20, getHeight() - 35);
    
    clearOutputButton.setBounds(10, statusReport.getBottom(), 75, 25);
}


void DataReportComponent::buttonClicked (Button* b)
{
    if(b == &clearOutputButton)
    {
        statusReport.setText("what!!!!!!!");
    }
}



