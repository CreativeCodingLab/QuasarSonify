/*
  ==============================================================================

    This file was auto-generated!

  ==============================================================================
*/

#pragma once

#include "../../JuceLibraryCode/JuceHeader.h"
#include "OSCHandler.h"
#include "../GlobalDefines.h"
#include "OverviewPlayback.h"
#include <algorithm>


#define RESOURCE_PATH (File::getSpecialLocation(File::SpecialLocationType::currentApplicationFile).getChildFile("Contents").getChildFile("Resources").getFullPathName())


#define DEFAULT_DATAFILE RESOURCE_PATH + "/TON580_norm.ascii"
//#define DEFAULT_DATAFILE RESOURCE_PATH + "/pg1260_norm.ascii"

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
    EFamilyID familyID{eFamilyID_none};
    double restframeRate{0};
    double oscillatorStrength{0};
    double absorptionProbability{0};
};

struct SpectralData
{
    int rawDataIndex{0};
    String displayName{""};
    String label{""};
    EFamilyID familyID{eFamilyID_none};
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
    
    
    // prep the atomic property list
    void loadAtomicDataList();
    void assignFamilyIDToAtom();
    void filterAtomicList_TON580(); // filtering atomic list based on criteria for initial QSS sonification development as outlined in JAES paper.
    void filterAtomicList_pg1260_Tripp(); // filtering atomic list to only include elements listed in paper "The Hidden Mass and Large Spatial Extent of a Post-Starburst Galaxy Outflow" - Todd M. Tripp (2011)
        
    
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
    
    void setBatchSpectralDataForSonification();
    

    // -----------------------------------------
    // ------------ Networking methods...
    // -----------------------------------------

    void broadcastDataOSC(std::vector<SpectralData> &specData)
    {
        // properties to send:
        // - element name
        // - restframe rate
        // - flux
        // - ACD
        // - spectral width and skew
        
        
        // sending a random spectral transition from the atom....
//        Random r;
//        int rIndex = r.nextInt((const int)specData.size());
//
//        String address = "/data/family_" + String(specData[rIndex].familyID);
//
//        // Broadcasting synth parameters:
//        // pitch
//        // amp
//        // attack
//        // decay
//        // sustain amp
//        // release
//        // stereo pan
//        String metricsString = String(60)
//                                + " " + String(1 - specData[rIndex].flux)
//                                + " " + String(1000)
//                                + " " + String(200)
//                                + " " + String(0.85)
//                                + " " + String(500)
//                                + " " + String(0.5);
//
//        oschandler.sendOSCMessageString(address + "/metrics", metricsString);

 
        
        
        // sending all spectral transitions from an atom.
        for(int i = 0; i < specData.size(); i++)
        {
            String address = "/data/family_" + String(specData[i].familyID);

            Random r;
            
            // Broadcasting synth parameters:
            // pitch
            // amp
            // attack
            // decay
            // sustain amp
            // release
            // stereo pan
            String metricsString = String(60 * pow(2, r.nextFloat()));
                                   + " " + String(1 - specData[i].flux)
                                   + " " + String(1000)
                                   + " " + String(200)
                                   + " " + String(0.85)
                                   + " " + String(500)
                                   + " " + String(r.nextFloat());

            oschandler.sendOSCMessageString(address + "/metrics", metricsString);

            oschandler.sendOSCMessageFloat("/data/reset", 1);
        }

        

        
        // original sending everything and labeling each metric.
        
//        oschandler.sendOSCMessageFloat("/data/reset", 1);

//        for(int i = 0; i < specData.size(); i++)
//        {
//            //std::cout << "--- broadcasting Atom: " << specData[i].label << ". with family ID: " << specData[i].familyID << std::endl;
//
//            String address = "/data/family_" + String(specData[i].familyID);
//
//            oschandler.sendOSCMessageString(address + "/Name", specData[i].label);
//            oschandler.sendOSCMessageFloat(address + "/restFrameRate", specData[i].restFrameRate);
//            oschandler.sendOSCMessageFloat(address + "/flux", specData[i].flux);
//            oschandler.sendOSCMessageFloat(address + "/amp", specData[i].mainAmpLevel);
//            oschandler.sendOSCMessageFloat(address + "/ACD", specData[i].ApparentColumnDensity);
//            oschandler.sendOSCMessageFloat(address + "/freq_lb", specData[i].bandwidthFreq_lb);
//            oschandler.sendOSCMessageFloat(address + "/freq_ub", specData[i].bandwidthFreq_ub);
//
//
//
//            String metricsString = String(specData[i].flux)
//                                    + " " + String(specData[i].ApparentColumnDensity)
//                                    + " " + String(specData[i].bandwidthFreq_lb);
//                                    + " " + String(specData[i].bandwidthFreq_ub);
//            oschandler.sendOSCMessageString(address + "/metrics", metricsString);
//        }
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
    
    std::unique_ptr<OverviewPlayback> overviewPlayback;

    
    
    // the raw wavelength vs flux data imported from file...
    std::vector<RawData> rawWavelengthData;
    
    // the master list of atomic data with corresonding spectral properties
    std::vector<AtomicTransitionProperties> atomicTransitions;
    
    // the organized / filtered list of atomic spectral properties
    std::vector<std::vector<SpectralData> > atomSpectraList;
    
    // analysis metrics stored in the SpectralData array for each element...
    std::vector<SpectralData> selectedSpectralData;

    // analysis metrics for multiple elements processed and stored in a batch.
    std::vector<std::vector<SpectralData> > selectedBatchSpectralData;
    
    
    float redShiftatVelocity{0};

    float minMaxVelocity_Absolute[2] = {0,0};
    float minMaxRedShift_Absolute[2] = {0,0};
    
    
    File loadedDataFile;
    

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DataHandler)
};
