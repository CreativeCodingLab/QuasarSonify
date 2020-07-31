/*
  ==============================================================================

    This file was auto-generated!

  ==============================================================================
*/

#pragma once

#include "../../JuceLibraryCode/JuceHeader.h"
#include "OSCHandler.h"
#include "../GlobalDefines.h"
#include <algorithm>


#define RESOURCE_PATH (File::getSpecialLocation(File::SpecialLocationType::currentApplicationFile).getChildFile("Contents").getChildFile("Resources").getFullPathName())

#define DEFAULT_REDSHIFT 0.20257
#define SPEEDOFLIGHT_C 299792.498



struct RawData
{
    double wavelength{0};
    double flux{0};
    double error{0};
};

struct AtomicTransitionProperties
{
    String name{""};
    double restframeRate{0};
    double oscillatorStrength{0};
    double absorptionProbability{0};
};

struct SpectralData
{
    int rawDataIndex{0};
    String displayName{""};
    String label{""};
    double restFrameRate{0}; //lyman value in context of hydrogen...
    double velocity{0};
    double oscillatorStrength{0};
    double absorptionProbability{0};
    double redShift{DEFAULT_REDSHIFT};
    double flux{0};
    double ApparentColumnDensity{0};
    
    double minVelocity{0};
    double maxVelocity{0};
    double minRedShift{0};
    double maxRedShift{0};

    
    double velocityHalfMax_lub{0};
    double velocityHalfMax_glb{0};
    
    double redShiftHalfMax_lub{0};
    double redShiftHalfMax_glb{0};
    
    
    // sonification parameters
    float mainAmpLevel{0};
    float centralFrequency{0};
    float bandwidthFreq_lb{0};
    float bandwidthFreq_ub{0};
    float skewFactor{1};
    float skewFrequency{0};
    float acdFrequency{0};
};



class DataHandler : public Thread
{
public:
    //==============================================================================
    DataHandler();
    ~DataHandler(){}
    
    // ------------ loading the data...
    
    void loadData(bool loadInThread = true);

    void run();
    
    String selectDataFileToLoad();

    void retrieveRawData(File dataFile);
    
    void loadAtomicDataList();
    
    void writeFilteredDataToFile();
    
    String printRawData();
    
    String printFilteredData();
    
    File getLoadedFile(){
        return loadedDataFile;
    }

    
    
    // -----------------------------------------
    // ------------ Data analysis methods...
    // -----------------------------------------

    void convertToHydrogenLymanVelocities();
    
    void getFluxFromRedShift(std::vector<SpectralData> &specData, float redShift);

    void getFluxFromVelocity(std::vector<SpectralData> &specData, float velocity, float redShift = 0.20257);
    
    float computeVelocityFromWavelength(float wavelength, float lymanX, float redShift);
    
    float setMinMaxLymanVelocities(std::vector<SpectralData> &specData);
    
    float setMinMaxLymanRedShift(std::vector<SpectralData> &specData);
    
    float boundaryAtHalfMaximum(int startIndx, SpectralData &specData);
        
    float computeRedShiftfromVelocity(float rawWavelength, float lymanValue);
    
    void setSpectralDataForAnalysis(int indx);
    

    // -----------------------------------------
    // ------------ Networking methods...
    // -----------------------------------------

    void broadcastDataOSC(std::vector<SpectralData> &specData)
    {
        //for(int i = 0; i < specData.size(); i++)
        {
            oschandler.sendOSCMessageFloat("/data/freq_ctr", specData[0].centralFrequency);
            oschandler.sendOSCMessageFloat("/data/freq_lb", specData[0].bandwidthFreq_lb);
            oschandler.sendOSCMessageFloat("/data/freq_ub", specData[0].bandwidthFreq_ub);
        }
    }
    
    // -----------------------------------------
    // ------------ Sonification methods...
    // -----------------------------------------

    void setSonificationSpectralAmplitudes(std::vector<SpectralData> &data);
    
    void setSonificationBandwidthParameters(std::vector<SpectralData> &data);
    
    void setACDFrequency(std::vector<SpectralData> &data);
    
    
    
    
    
    // -----------------------------------------
    // ------------ Class Members...
    // -----------------------------------------

    OSCHandler oschandler;
    
    
    // the raw wavelength vs flux data imported from file...
    std::vector<RawData> rawWavelengthData;
    
    // the master list of atomic data with corresonding spectral properties
    std::vector<AtomicTransitionProperties> atomicTransitions;
    
    // the organized / filtered list of atomic spectral properties
    std::vector<std::vector<SpectralData> > atomSpectraList;
    
    // analysis metrics stored in the SpectralData array for each element...
    std::vector<SpectralData> selectedSpectralData;

    
    
    float redShiftatVelocity{0};

    float minMaxVelocity_Absolute[2] = {0,0};
    float minMaxRedShift_Absolute[2] = {0,0};
    
    
    File loadedDataFile;
    

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DataHandler)
};
