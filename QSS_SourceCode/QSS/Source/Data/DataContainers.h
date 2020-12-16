/*
  ==============================================================================

    This file was auto-generated!

  ==============================================================================
*/

#pragma once

#include "../../JuceLibraryCode/JuceHeader.h"
#include "../GlobalDefines.h"



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


