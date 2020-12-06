
#pragma once


#include "../JuceLibraryCode/JuceHeader.h"
#include "../GlobalDefines.h"
#include "../Data/DataHandler.h"
#include "../Libraries/Audio/Biquads.h"
#include "../Libraries/Audio/Oscillators.h"
#include "../Libraries/Audio/SmoothGain.h"
#include "../Data/SensoryDissonance.h"

enum EWidthTimerType
{
    eWidthTimer_alpha = 1,
    eWidthTimer_beta,
    eWidthTimer_gamma
};


class AudioSpectralElementProcessor
{
    
public:
    
    AudioSpectralElementProcessor()
    {
        mainOscil.setOscillatorType(eOscillatorType_Sine);
        mainOscil.setFrequency(440 / 2);

        skewOscil.setOscillatorType(eOscillatorType_Sine);
        skewOscil.setFrequency(440 / 2);
        
        acdOscil.setOscillatorType(eOscillatorType_Sine);
        acdOscil.setFrequency(440 / 2);

        upperBandWidthOscil.setOscillatorType(eOscillatorType_Sine);
        upperBandWidthOscil.setFrequency(440 / 2);

        lowerBandWidthOscil.setOscillatorType(eOscillatorType_Sine);
        lowerBandWidthOscil.setFrequency(440 / 2);
        
        lowPass.setLowPass(8000, 0.707, true);
    }
    ~AudioSpectralElementProcessor()
    {
        
    }
    
    void processSpectralAudio(AudioBuffer<float> *audioBuffer)
    {
        bufMain.makeCopyOf(*audioBuffer);
        if(enableSkew) bufSkew.makeCopyOf(*audioBuffer);
        if(enableACD) bufACD.makeCopyOf(*audioBuffer);
        bufUBand.makeCopyOf(*audioBuffer);
        bufLBand.makeCopyOf(*audioBuffer);

        mainOscil.processOscillator(&bufMain);
        if(enableSkew) skewOscil.processOscillator(&bufSkew);
        if(enableACD) acdOscil.processOscillator(&bufACD);
        upperBandWidthOscil.processOscillator(&bufUBand);
        lowerBandWidthOscil.processOscillator(&bufLBand);

        bufUBand.applyGain(ampLevel_uppperBW);
        bufLBand.applyGain(ampLevel_lowerBW);
        
        // apply smoothed gain to the audio buffers...
        sGain_main.processGain(&bufMain);
        sGain_skew.processGain(&bufSkew);
        sGain_acd.processGain(&bufACD);
        

        for(int i = 0; i < audioBuffer->getNumChannels(); i++)
        {
            audioBuffer->addFrom(i, 0, bufMain, i, 0, audioBuffer->getNumSamples());
            if(enableSkew)
                audioBuffer->addFrom(i, 0, bufSkew, i, 0, audioBuffer->getNumSamples());
            if(enableACD)
                audioBuffer->addFrom(i, 0, bufACD, i, 0, audioBuffer->getNumSamples());
            audioBuffer->addFrom(i, 0, bufUBand, i, 0, audioBuffer->getNumSamples());
            audioBuffer->addFrom(i, 0, bufLBand, i, 0, audioBuffer->getNumSamples());
        }
        
        
        lowPass.processBuffer(*audioBuffer);
    }
    
    void setCentralFreqForAllOscillators(float freq)
    {
        mainOscil.setFrequency(freq);
        skewOscil.setFrequency(freq);
        acdOscil.setFrequency(freq);
        upperBandWidthOscil.setFrequency(freq);
        lowerBandWidthOscil.setFrequency(freq);
    }
    
    void setTypeForAllOscillators(EOscillatorType oscType)
    {
        mainOscil.setOscillatorType(oscType);
        skewOscil.setOscillatorType(oscType);
        acdOscil.setOscillatorType(oscType);
        upperBandWidthOscil.setOscillatorType(oscType);
        lowerBandWidthOscil.setOscillatorType(oscType);
    }

    
    Oscillators mainOscil;
    Oscillators skewOscil;
    Oscillators acdOscil;
    Oscillators upperBandWidthOscil;
    Oscillators lowerBandWidthOscil;
    
    AudioSampleBuffer bufMain;
    AudioSampleBuffer bufSkew;
    AudioSampleBuffer bufACD;
    AudioSampleBuffer bufUBand;
    AudioSampleBuffer bufLBand;
    
    SmoothGain sGain_main;
    SmoothGain sGain_skew;
    SmoothGain sGain_acd;
    
    
    
    Biquads lowPass;


    float ampLevel_main{0};
    float ampLevel_uppperBW{0};
    float ampLevel_lowerBW{0};

    float centralFreuqency{0};
    float upperBoundFreq{0};
    float lowerBoundFreq{0};
    float skewFreq{0};
    
    float upperProportion{0};
    float lowerProportion{0};
    
    
    
    // set to false if there is no data for this element or if the redshift/velocity value is out of range.
    bool isInRangeForProcessing{false};
    
    // set from the view
    bool enableSkew{false};
    bool enableACD{false};
    bool enableProcessing{true};

    
    int incrTime{0};
    int timerID{0};
};



class AudioGraph : public MultiTimer
{
public:
    
    
    virtual void timerCallback (int timerID) override
    {
        
        for(int i = 0; i < spectralElementProcessors.size(); i++)
        {
            if(timerID == spectralElementProcessors[i].timerID)
            {
                int incrTime = spectralElementProcessors[i].incrTime;
                float ubFreq = spectralElementProcessors[i].upperBoundFreq;
                float lbFreq = spectralElementProcessors[i].lowerBoundFreq;
                float cntrFreq = spectralElementProcessors[i].centralFreuqency;
                float upperProp = spectralElementProcessors[i].upperProportion;
                float lowerProp = spectralElementProcessors[i].lowerProportion;
                
                if(incrTime < 1000)
                {
                    float glissTimeUp = upperProp;
                    float glissTimeDown = lowerProp;
                    float totalTime_ms = 1000;
                    
                    float amp = 0;
                    float attackDecayTime = glissTimeUp * totalTime_ms;
                    if(incrTime < attackDecayTime*0.1)
                    {
                        amp = incrTime / (attackDecayTime*0.1);
                    }
                    else if(incrTime < attackDecayTime * 0.9)
                    {
                        amp = 1;
                    }
                    else if(incrTime < attackDecayTime)
                    {
                        amp = 1 - (incrTime - attackDecayTime * 0.9) / (attackDecayTime*0.1);
                    }
                    spectralElementProcessors[i].ampLevel_uppperBW = amp*spectralElementProcessors[i].ampLevel_main;
                    if(incrTime <= attackDecayTime)
                    {
                        float inputFreq = cntrFreq  + (incrTime / attackDecayTime) * ubFreq;
                        spectralElementProcessors[i].upperBandWidthOscil.setFrequency(inputFreq);
                    }
                    
                    amp = 0;
                    attackDecayTime = glissTimeDown * totalTime_ms;
                    if(incrTime < attackDecayTime * 0.1)
                    {
                        amp = incrTime / (attackDecayTime * 0.1);
                    }
                    else if(incrTime < attackDecayTime * 0.9)
                    {
                        amp = 1;
                    }
                    else if(incrTime < attackDecayTime)
                    {
                        amp = 1 - (incrTime - attackDecayTime * 0.9) / (attackDecayTime * 0.1);
                    }
                    spectralElementProcessors[i].ampLevel_lowerBW = amp*spectralElementProcessors[i].ampLevel_main;
                    if(incrTime <= attackDecayTime)
                    {
                        float inputFreq = cntrFreq  - (incrTime / attackDecayTime) * lbFreq;
                        spectralElementProcessors[i].lowerBandWidthOscil.setFrequency(inputFreq);
                    }
                    
                    spectralElementProcessors[i].incrTime += 1;
                }
                else
                {
                    spectralElementProcessors[i].incrTime = 0;
                    stopTimer(spectralElementProcessors[i].timerID);
                }
            }
        }
    }
    
    void triggerGliss_alpha()
    {
        spectralElementProcessors[0].incrTime = 0;
        startTimer(spectralElementProcessors[0].timerID, 1);
    }
    
    void triggerGliss_beta()
    {
        spectralElementProcessors[1].incrTime = 0;
        startTimer(spectralElementProcessors[1].timerID, 1);
    }

    void triggerGliss_gamma()
    {
        spectralElementProcessors[2].incrTime = 0;
        startTimer(spectralElementProcessors[2].timerID, 1);
    }

    
    
    
    AudioGraph(int numChannels)
    {
        for(int i = 0; i < 3; i++)
        {
            AudioSpectralElementProcessor asep;
            spectralElementProcessors.push_back(asep);
        }
        spectralElementProcessors[0].setCentralFreqForAllOscillators(440/2);
        spectralElementProcessors[0].setTypeForAllOscillators(eOscillatorType_Sine);
        spectralElementProcessors[0].timerID = 1;
        
        spectralElementProcessors[1].setCentralFreqForAllOscillators(440/2 * 1.5);
        spectralElementProcessors[1].setTypeForAllOscillators(eOscillatorType_Square);
        spectralElementProcessors[1].timerID = 2;
        
        spectralElementProcessors[2].setCentralFreqForAllOscillators(440/2 * 1.25);
        spectralElementProcessors[2].setTypeForAllOscillators(eOscillatorType_Saw);
        spectralElementProcessors[2].timerID = 3;
        
    }
    
    ~AudioGraph(){}
    
    
    void processGraph(AudioBuffer<float> *audioBuffer, int numChannels)
    {
        if(!audioOn) return;
        
        audioBuffer->applyGain(0.0);
        
        
        for(int i = 0 ; i < spectralElementProcessors.size(); i++)
        {
            if(spectralElementProcessors[i].enableProcessing && spectralElementProcessors[i].isInRangeForProcessing)
            {
                spectralElementProcessors[i].processSpectralAudio(audioBuffer);
            }
        }

        
        audioBuffer->applyGain(outputLevel);
    }
    
    
    void mapSonificationParameters(DataHandler* data)
    {
        //std::cout << "------------------ spectral sound parameters. ElementSize: " << data->selectedSpectralData.size() << std::endl;

        for(int i = 0; i < spectralElementProcessors.size(); i++)
        {
            float mainAmp = 0;
            float skewFreq = 0;
            float acdFreq = 0;
            float centralFreq = 0;
            float ubFreq = 0;
            float lbFreq = 0;
            float upperProp = 0;
            float lowerProp = 0;
            bool inRange = false;
            
            
            if(i < data->selectedSpectralData.size())
            {
                mainAmp = data->selectedSpectralData[i].mainAmpLevel;
                skewFreq = data->selectedSpectralData[i].skewFrequency;
                acdFreq = data->selectedSpectralData[i].acdFrequency;
                centralFreq = data->selectedSpectralData[i].centralFrequency;
                ubFreq = data->selectedSpectralData[i].bandwidthFreq_ub;
                lbFreq = data->selectedSpectralData[i].bandwidthFreq_lb;
                upperProp = (data->selectedSpectralData[i].redShiftHalfMax_lub - data->selectedSpectralData[i].redShift) / (data->selectedSpectralData[i].redShiftHalfMax_lub - data->selectedSpectralData[i].redShiftHalfMax_glb);
                lowerProp = (data->selectedSpectralData[i].redShift - data->selectedSpectralData[i].redShiftHalfMax_glb) / (data->selectedSpectralData[i].redShiftHalfMax_lub - data->selectedSpectralData[i].redShiftHalfMax_glb);

                inRange = true;
            }
            
            // adjust amplitude for equal loudness perception as follows:
            // - convert amp to dB (based on ref amp of 0.00001)
            // - calcuate phon level at base frequency (220 Hz)
            // - calculate adjusted dB level based on phon and central frequency
            // - convert dB back to amplitude.
            float amp_eqLoudness = adjustAmpforEqualLoudness(mainAmp, centralFreq);
            
            //std::cout << "********* freq: " << centralFreq <<  "--- main amp v adjusted amp: " << mainAmp << ", " << amp_eqLoudness << std::endl;
            
            // assign parameters to element processor
            spectralElementProcessors[i].ampLevel_main = amp_eqLoudness;
            spectralElementProcessors[i].sGain_main.setGain(amp_eqLoudness);
            spectralElementProcessors[i].sGain_skew.setGain(amp_eqLoudness*0.8);
            spectralElementProcessors[i].sGain_acd.setGain(amp_eqLoudness*0.8);

            spectralElementProcessors[i].skewOscil.setFrequency(skewFreq);
            spectralElementProcessors[i].acdOscil.setFrequency(acdFreq);
            spectralElementProcessors[i].centralFreuqency = centralFreq;
            spectralElementProcessors[i].upperBoundFreq = ubFreq;
            spectralElementProcessors[i].lowerBoundFreq = lbFreq;
            spectralElementProcessors[i].isInRangeForProcessing = inRange;
            spectralElementProcessors[i].upperProportion = upperProp;
            spectralElementProcessors[i].lowerProportion = lowerProp;

            
//            std::cout << "--- spectral transition: " << i << std::endl;
//            std::cout << "amp: " <<  mainAmp << std::endl;
//            std::cout << "freq: " << centralFreq << std::endl;
//            std::cout << "upper bound freq: " << ubFreq << std::endl;
//            std::cout << "lower boun freq: " << lbFreq << std::endl;
//            std::cout << "skewFreq: " << skewFreq << std::endl;
//            std::cout << "enable: " << inRange << std::endl;
            
        }
    }
    
    
    void setSpectralElementProcessors(int numProcessors)
    {
    }
    
    float adjustAmpforEqualLoudness(float amp, float centralFreq)
    {
        float refAmp = 0.00001;
        
        if(amp <= refAmp)
        {
            return amp;
        }
        
        float dB_base = 20*log10(amp/refAmp);
        
        float basePhon = sensoryDiss.dBtoPhons(220, dB_base);
        
        float dB_adjusted = sensoryDiss.PhontodB(centralFreq, basePhon);
        
        float amp_adjusted = powf(10, dB_adjusted / 20) * refAmp;
        
        return amp_adjusted;
    }
    
    
    void setInputDrive(float drive)
    {
        //NOTE: express inputDrive in terms of dB
        inputDrive = drive * 36.0 - 18.0;
        
        //now convert dB to Amp
        inputDrive = powf(10, inputDrive/20);
    }
    
    void setOutputLevel(float level)
    {
        //NOTE:express outputLevel in terms of dB
        //outputLevel = level * 36.0 - 18.0;
        
        //now convert dB to Amp
        outputLevel = level; //powf(10, outputLevel/20);
    }
    
    
    float getOutputLevel(){return outputLevel;}
    
    void setBypassGraph(bool bypass) {bypassGraph = bypass;}
    bool isGraphBypassed(){return bypassGraph;}
    
    

    bool bypassGraph{false};
    float inputDrive{0.0};
    float outputLevel{0.0};
           
    bool audioOn{false};
    
    
    
    std::vector<AudioSpectralElementProcessor> spectralElementProcessors;
    
    SensDissonance sensoryDiss;

};


