
#pragma once


#define CURRENT_EXECUTABLE_PATH (File::getSpecialLocation(File::currentExecutableFile).getParentDirectory().getParentDirectory().getFullPathName())
//#define TEST_AUDIO_PATH CURRENT_EXECUTABLE_PATH + "/Resources/Audio"
#define TEST_AUDIO_PATH "/Users/Resources/Concatenator/Audio"

#define GUI_PATH "/Resources/GLContent"

#define BATCH_SONIFY_TEST 0

#define DEFAULT_REDSHIFT 0.20257
#define SPEEDOFLIGHT_C 299792.498


enum EFamilyID
{
    eFamilyID_none,
    eFamilyID_1,
    eFamilyID_2,
    eFamilyID_3,
    eFamilyID_4,
    eFamilyID_5,
    eFamilyID_6,
    eFamilyID_7,
    eFamilyID_8,
    eFamilyID_9,
    eFamilyID_10,
    
    eFamilyID_all
};
