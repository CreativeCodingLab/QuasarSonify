/*
  ==============================================================================

    This file was auto-generated!

  ==============================================================================
*/

#include "SpectrumPlotComponent.h"

//==============================================================================


SpectrumPlotComponent::SpectrumPlotComponent()
{
    Random r;
    
//    for(int i = 0; i < 50; i++)
//    {
//        spectralPoint sp;
//        sp.wavelength = 0;
//        sp.flux = r.nextFloat();
//
//        spectralPlot.push_back(sp);
//    }

}


void SpectrumPlotComponent::formatPlotAndRepaint()
{
    // format the plot for our view...
    float yMax = -999;
    float yMin = 999;
    float minMaxX[2] = {9999, -9999};
    for(int i = 0; i < spectralPlot.size(); i++)
    {
        if(spectralPlot[i].flux > yMax)
        {
            yMax = spectralPlot[i].flux;
        }
        if(spectralPlot[i].flux < yMin)
        {
            yMin = spectralPlot[i].flux;
        }
    }
    
    // normalize the y-dimension of the plot
    for(int i = 0; i < spectralPlot.size(); i++)
    {
        float topMargin = 30;
        float bottomMargin = 0;
        float yWindow = getHeight() - topMargin - bottomMargin;
        
        spectralPlot[i].curY = topMargin + yWindow * (1 - spectralPlot[i].flux);
    }
    
    repaint();
}


void SpectrumPlotComponent::paint(Graphics& g)
{
    g.setColour(Colours::black.withAlpha(0.65f));
    g.fillAll();
    
    // the zero horizontal line and red shift marker
    g.setColour(Colours::red);
    g.drawHorizontalLine(30, 0, getWidth());
    
    
    // draw the absorption plot
    if(xCenter3 > 0)
    {
        int startIndx = xCenter3 - xWidth / 2;
        float windowSize = xWidth;

        // draw the path of the plot
        float xIncr = (float)getWidth() / windowSize;//(float)spectralPlot.size() ;
        float xPos = 0;
        Path p;
        p.startNewSubPath(0, spectralPlot[startIndx].curY);
        xPos += xIncr;
        for(int i = startIndx + 1; i < startIndx + windowSize; i++)
        {
            p.lineTo(xPos, spectralPlot[i].curY);

            xPos += xIncr;
        }
        p = p.createPathWithRoundedCorners(5);

        g.setColour(Colours::white.withAlpha(0.2f));
        g.strokePath(p, PathStrokeType (1.0));
    }
    
    if(xCenter2 > 0)
    {
        int startIndx = xCenter2 - xWidth / 2;
        float windowSize = xWidth;

        // draw the path of the plot
        float xIncr = (float)getWidth() / windowSize;//(float)spectralPlot.size() ;
        float xPos = 0;
        Path p;
        p.startNewSubPath(0, spectralPlot[startIndx].curY);
        xPos += xIncr;
        for(int i = startIndx + 1; i < startIndx + windowSize; i++)
        {
            p.lineTo(xPos, spectralPlot[i].curY);

            xPos += xIncr;
        }
        p = p.createPathWithRoundedCorners(5);

        g.setColour(Colours::white.withAlpha(0.5f));
        g.strokePath(p, PathStrokeType (1.0));
    }
    
    float verticalLineX = 0;
    if(xCenter1 > 0)
    {
        int startIndx = xCenter1 - xWidth / 2;
        float windowSize = xWidth;
        
        // draw the path of the plot
        float xIncr = (float)getWidth() / windowSize;//(float)spectralPlot.size() ;
        float xPos = 0;
        Path p;
        p.startNewSubPath(0, spectralPlot[startIndx].curY);
        xPos += xIncr;
        for(int i = startIndx + 1; i < startIndx + windowSize; i++)
        {
            if(i == xCenter1)
            {
                verticalLineX = xPos;
            }
            p.lineTo(xPos, spectralPlot[i].curY);
            
            xPos += xIncr;
        }
        p = p.createPathWithRoundedCorners(5);
        
        g.setColour(Colours::white);
        g.strokePath(p, PathStrokeType (1.0));
    }
    
    
    g.setColour(Colours::red);
    g.drawVerticalLine(verticalLineX, 0, getHeight());
}


void SpectrumPlotComponent::resized()
{

}


