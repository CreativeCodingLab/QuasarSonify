
#pragma once


#include "../JuceLibraryCode/JuceHeader.h"
#include "../GlobalDefines.h"
#include "OSCHandler.h"
#include "DataContainers.h"


struct PlaybackParameters
{
    EFamilyID familyID{eFamilyID_none};
    
    int playbackSpeedMS{100};
    int randomSpeedOffset{0};
    
    int playbackCounter{0};
    
    bool playbackAudio{false};
    
    int metricIndx{0};
    
    struct NoteParameters
    {
        int pitch{0};
        float amp{0};
        float attack{0};
        float decay{0};
        float sustain{1.};
        float release{0};
        float pan{0};
    };
    std::vector<NoteParameters> noteList;
            
    
    String familyAddress{""};
    StringArray metricStrings;
};



class OverviewPlayback : public HighResolutionTimer
{
    
public:
    
    OverviewPlayback(OSCHandler *osc_) : osc(osc_)
    {
        for(int i = 0; i < eFamilyID_all; i++)
        {
            PlaybackParameters pp;
            familyPlayback.push_back(pp);
        }
    }
    
    ~OverviewPlayback(){}
    
    
    void hiResTimerCallback ()
    {
        //
        for(int i = 0; i < familyPlayback.size(); i++)
        {
            if(familyPlayback[i].playbackAudio)
            {
                if(familyPlayback[i].playbackCounter == 0)
                {
                    int indx = familyPlayback[i].metricIndx;
                    
                    // make some random adjustments to the note list for the family.
                    noteListAdjustments(indx, familyPlayback[i]);
                    
                    
                    // get the address and note metric string and broadcast over osc...
                    String address = familyPlayback[i].familyAddress;
                    String metrics = familyPlayback[i].metricStrings[indx];
                    osc->sendOSCMessageString(address, metrics);
                    
                    
                    // inrement the note list index for this family...
                    familyPlayback[i].metricIndx = (indx + 1) % familyPlayback[i].metricStrings.size();
                    
                    
                    // add rsome andomness to the playback onset time for each tone...
                    Random r;
                    familyPlayback[i].randomSpeedOffset = 1.0 * r.nextFloat() * familyPlayback[i].playbackSpeedMS - 0.5 * familyPlayback[i].playbackSpeedMS;
                }

                // keep track of playback time... note can randomize a bit...
                int playbackTime_ms = familyPlayback[i].playbackSpeedMS + familyPlayback[i].randomSpeedOffset;
                familyPlayback[i].playbackCounter = (familyPlayback[i].playbackCounter + 1) % playbackTime_ms;
                //std::cout << "----- counter: " << familyPlayback[i].playbackCounter << std::endl;
            }
        }
    }
    
    
    void setFamilyPlaybackSpeed(EFamilyID famID, int playSpeedMS)
    {
        for(int i = 0; i < familyPlayback.size(); i++)
        {
            if(familyPlayback[i].familyID == famID)
            {
                familyPlayback[i].playbackSpeedMS = playSpeedMS;
            }
        }
    }
    
    
    void startFamilyPlayback(EFamilyID famID)
    {
        for(int i = 0; i < familyPlayback.size(); i++)
        {
            if(familyPlayback[i].familyID == famID)
            {
                familyPlayback[i].playbackAudio = true;
            }
        }
    }
    
    
    void stopFamilyPlayback(EFamilyID  famID)
    {
        for(int i = 0; i < familyPlayback.size(); i++)
        {
            if(familyPlayback[i].familyID == famID)
            {
                familyPlayback[i].playbackAudio = false;
            }
        }
    }
    
    
    void startStopPlaybackAll(bool start)
    {
        for(int i = 0; i < familyPlayback.size(); i++)
        {
            familyPlayback[i].playbackAudio = start;
        }

        if(start)
        {
            startTimer(1);
        }
        else
        {
            stopTimer();
        }
    }
    
    
    
    // main call to set the overview playback...
    void setMainOverviewPlayback(std::vector<std::vector<SpectralData> > &selectedBatchSpectralData);
    
    // this family consists of hydrogen (H1) ion
    void setOverviewPlayback_Family1(std::vector<SpectralData> &specData);
    
    // this family consists of other ions: ...to be determined...
    void setOverviewPlayback_Family2(std::vector<SpectralData> &specData);

    
    // used to make random adjustments to a given family's note list during playback.
    void noteListAdjustments(int noteIndx, PlaybackParameters &famPlayback);

    
    
    

    std::vector<PlaybackParameters> familyPlayback;
    
    OSCHandler *osc{nullptr};

};


