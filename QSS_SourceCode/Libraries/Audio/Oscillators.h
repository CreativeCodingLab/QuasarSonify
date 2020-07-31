#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "AudioGlobalDefines.h"
#include <vector>

#define WAVETABLE_SIZE 1024

enum EOscillatorType
{
    eOscillatorType_Sine,
    eOscillatorType_Saw,
    eOscillatorType_Square,
    eOscillatorType_Triangle,
    eOscillatorType_Custom
};


class Oscillators
{
public:
    
    
    Oscillators() : tableIndx(0.0), tableIncr(0.0), oscillatorType(eOscillatorType_Sine), polyBLEP_dt(0)
    {
        for(int i = 0; i < WAVETABLE_SIZE; i++)
        {
            sineTable.push_back(sin((float)i/(float)WAVETABLE_SIZE*2*PI));
            sawTable.push_back(2.0*float(i)/(float)WAVETABLE_SIZE - 1.0);
            squareTable.push_back(i < WAVETABLE_SIZE/2 ? 1.0 : -1.0);
            if(i < WAVETABLE_SIZE / 2){
                float gradient = (float)i / ((float)WAVETABLE_SIZE/2.0);
                triangleTable.push_back(2.0 * gradient - 1.0);
            } else {
                float gradient = 1.0 - ((float)i - (float)WAVETABLE_SIZE/2.0) / ((float)WAVETABLE_SIZE/2.0);
                triangleTable.push_back(2.0 * gradient - 1.0);
            }
            customTable.push_back(0.0);
            currentlyActiveTable.push_back(sin((float)i/(float)WAVETABLE_SIZE*2*PI));
        }
    }
    
    
    float processOscillator(AudioBuffer<float> *aBuffer)
    {
        float outSample = 0;
        
        for(int i = 0; i < aBuffer->getNumSamples(); i++)
        {
            for(int j = 0; j < aBuffer->getNumChannels(); j++)
            {
                if(j == 0)
                {
                    outSample = processOscillatorSample();
                }
                
                aBuffer->setSample(j, i, outSample);
            }
        }
    }

    
    float processOscillatorSample()
    {
        float outSample = 0;

        int priorIndx = (int)tableIndx;
        int nxtIndx = priorIndx + 1;
        if(nxtIndx >= WAVETABLE_SIZE) nxtIndx -= WAVETABLE_SIZE;
        float fracIndx = tableIndx - priorIndx;
        
        outSample = (1.0 - fracIndx)*currentlyActiveTable[priorIndx] + (fracIndx)*currentlyActiveTable[nxtIndx];
        
        tableIndx = (tableIndx + tableIncr);
        
        if(tableIndx >= WAVETABLE_SIZE) tableIndx -= WAVETABLE_SIZE;
        
        return outSample;

    }
    
    
    
    double doPolyBLEP(float curIndex)
    {
        int priorIndx = (int)curIndex;
        int nxtIndx = priorIndx + 1;
        if(nxtIndx >= WAVETABLE_SIZE) nxtIndx -= WAVETABLE_SIZE;
        float fracIndx = curIndex - priorIndx;

    
        float prior_t = (float)priorIndx / (float)WAVETABLE_SIZE;
        float nxt_t = (float)nxtIndx / (float)WAVETABLE_SIZE;
        
        float priorAdjVal = 0.0;
        float nxtAdjVal = 0.0;
        
        
        //Calcuate prior index BLEP adjustment
        if (prior_t < polyBLEP_dt) // 0 <= t < 1
        {
            prior_t /= polyBLEP_dt;
            // 2 * (t - t^2/2 - 0.5)
            priorAdjVal = prior_t+prior_t - prior_t*prior_t - 1.;
        }
        else if (prior_t > 1. - polyBLEP_dt) // -1 < t < 0
        {
            prior_t = (prior_t - 1.) / polyBLEP_dt;
            // 2 * (t^2/2 + t + 0.5)
            priorAdjVal = prior_t*prior_t + prior_t+prior_t + 1.;
        }
        else
        {
            priorAdjVal = 0.;
        }
        
        
        //Calcuate nxt index BLEP adjustment
        if (nxt_t < polyBLEP_dt) // 0 <= t < 1
        {
            nxt_t /= polyBLEP_dt;
            // 2 * (t - t^2/2 - 0.5)
            nxtAdjVal = nxt_t+nxt_t - nxt_t*nxt_t - 1.;
        }
        else if (nxt_t > 1. - polyBLEP_dt) // -1 < t < 0
        {
            nxt_t = (nxt_t - 1.) / polyBLEP_dt;
            // 2 * (t^2/2 + t + 0.5)
            nxtAdjVal = nxt_t*nxt_t + nxt_t+nxt_t + 1.;
        }
        else
        {
            nxtAdjVal = 0.;
        }

        
        return (1.0 - fracIndx)*priorAdjVal + (fracIndx)*nxtAdjVal;
    }
    
    
    void setPolyBLEPdt(float freq)
    {
        polyBLEP_dt = freq/SAMPLE_RATE*2.0;
    }
    
    void setFrequency(float freq)
    {
        tableIncr = (float)WAVETABLE_SIZE/(float)SAMPLE_RATE*freq;
        setPolyBLEPdt(freq);
    }
    
    
    void setOscillatorType(EOscillatorType oscType)
    {
        oscillatorType = oscType;
        
        currentlyActiveTable.clear();
        
        for(int i = 0; i < WAVETABLE_SIZE; i++)
        {
            switch (oscillatorType)
            {
                case eOscillatorType_Sine:
                    currentlyActiveTable.push_back(sineTable[i]);
                    break;
                case eOscillatorType_Saw:
                    currentlyActiveTable.push_back(sawTable[i]);
                    break;
                case eOscillatorType_Square:
                    currentlyActiveTable.push_back(squareTable[i]);
                    break;
                case eOscillatorType_Triangle:
                    currentlyActiveTable.push_back(triangleTable[i]);
                    break;
                case eOscillatorType_Custom:
                    currentlyActiveTable.push_back(customTable[i]);
                    break;
                    
                default:
                    break;
            }
        }

    }
    
    
    void setCustomTable(std::vector<float> table)
    {
        customTable = table;
    }
    
    
private:
    
    
    EOscillatorType oscillatorType;
    
    std::vector<float> sineTable;
    std::vector<float> sawTable;
    std::vector<float> squareTable;
    std::vector<float> triangleTable;
    std::vector<float> customTable;
    std::vector<float> currentlyActiveTable;
    
    
    float tableIncr{0};
    float tableIndx{0};
    double polyBLEP_dt{0};
};


