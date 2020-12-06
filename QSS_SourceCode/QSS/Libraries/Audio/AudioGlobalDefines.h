//
//  Header.h
//  STKBasedGranulator
//
//  Created by Brian Hansen on 2/3/16.
//
//

#pragma once


#define GRIDSIZE 310
#define GRIDRESOLUTION 720


#define CURRENT_EXECUTABLE_PATH (File::getSpecialLocation(File::currentExecutableFile).getParentDirectory().getParentDirectory().getFullPathName())
//#define TEST_AUDIO_PATH CURRENT_EXECUTABLE_PATH + "/Resources/Audio"
#define TEST_AUDIO_PATH "/Users/Resources/Concatenator/Audio"

#define GUI_PATH "/Resources/GLContent"



#define USE_TEST_AUDIO 1


#define PI (3.14159265359)
#define SAMPLE_RATE 44100

#define ANALYSIS_BUFFER_SIZE 44100
#define MAX_GRAIN_SIZE 44100


#define DEFAULT_ANALYSIS_SIZE 10000

#define AMP_MATCH_FLOOR 0.01
#define RMS_FLOOR 0.005

const float noMetricTypeValue = -12345;


enum eTuningMode
{
    eTuning_Chromatic,
    eTuning_Diatonic,
    eTuning_Overtone
};



struct SpectrumData
{
    float frequency;
    float magnitude;
};


