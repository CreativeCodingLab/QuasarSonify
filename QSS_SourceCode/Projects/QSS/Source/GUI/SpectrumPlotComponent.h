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

struct spectralPoint
{
    float wavelength{0};
    float flux{0};
    
    float curX{0};
    float curY{0};
};

class SpectrumPlotComponent : public Component
{
public:
    
    SpectrumPlotComponent();
    ~SpectrumPlotComponent(){}
    
    void paint(Graphics &g) override;
    void resized() override;
    
    void formatPlotAndRepaint();
    
    void setSpectralPlotData(std::vector<RawData> &rawData)
    {
        for(int i = 0; i < rawData.size(); i++)
        {
            spectralPoint sp;
            sp.wavelength = rawData[i].wavelength;
            sp.flux = rawData[i].flux;
            
            spectralPlot.push_back(sp);
        }
        
        
        formatPlotAndRepaint();
    }
    
    void plotCenterAndWidth(int center1, int center2, int center3, int width)
    {
        xCenter1 = center1;
        xCenter2 = center2;
        xCenter3 = center3;
        xWidth = width;
        
        repaint();
    }
    
    
private:
    
    std::vector<spectralPoint> spectralPlot;
    
    int xCenter1{0};
    int xCenter2{0};
    int xCenter3{0};
    int xWidth{0};

    
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SpectrumPlotComponent)
};




