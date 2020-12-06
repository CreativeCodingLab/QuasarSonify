#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "AudioGlobalDefines.h"
#include <vector>


// This class is a smooth sounding gain (i.e. no crackels when adjusting the levels
class SmoothGain
{
public:
    
    SmoothGain()
    {}
    
    
    float processGain(AudioBuffer<float> *aBuffer)
    {
        float outSample = 0;
        
        for(int i = 0; i < aBuffer->getNumSamples(); i++)
        {
            for(int j = 0; j < aBuffer->getNumChannels(); j++)
            {
                outSample = aBuffer->getSample(j, i);
                
                if(fabs(currentGain - targetGain) < 0.0005){
                    currentGain = targetGain;
                }

                outSample = outSample*currentGain;
                
                if(j == 0){
                    currentGain = currentGain + (targetGain - currentGain)*.0005;
                }
                
                aBuffer->setSample(j, i, outSample);
            }
        }
    }

    
    float processGainSample(float sample)
    {
        if(fabs(currentGain - targetGain) < 0.0005){
            currentGain = targetGain;
        }
        
        sample = sample*currentGain;
        
        currentGain = currentGain + (targetGain - currentGain)*.0005;
        
        return sample;
    }
    
    
    void setGain(float gain)
    {
        targetGain = gain;
    }
    
    
    
private:
    
    float currentGain{0.};
    float targetGain{0.};

};


