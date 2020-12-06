
#pragma once


#include "../JuceLibraryCode/JuceHeader.h"
#include "../GlobalDefines.h"
#include "OSCHandler.h"


class OverviewPlayback : public MultiTimer
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
    
    ~OverviewPlayback()
    {
        
    }
    
    
    void timerCallback (int timerID)
    {
        
        // use the EFamilyID as the timerID, so we can filter on that to check what should be broadcast...
        for(int i = 0; i < familyPlayback.size(); i++)
        {
            if(familyPlayback[i].familyID == (EFamilyID)timerID)
            {
                
                String address = familyPlayback[i].familyAddress;
                int indx = familyPlayback[i].metricIndx;
                String metrics = familyPlayback[i].metricStrings[indx];
                
                // broadcast the above address and metrics....
                osc->sendOSCMessageString(address, metrics);
                
                familyPlayback[i].metricIndx = (indx + 1) % familyPlayback[i].metricStrings.size();
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
                startTimer(famID, familyPlayback[i].playbackSpeedMS);
            }
        }
    }
    
    
    void stopFamilyPlayback(EFamilyID  famID)
    {
        stopTimer(famID);
    }
    
    
    void startStopPlaybackAll(bool start)
    {
        if(start)
        {
            for(int i = 0; i < familyPlayback.size(); i++)
            {
                startTimer(familyPlayback[i].familyID, familyPlayback[i].playbackSpeedMS);
            }
        }
        else
        {
            for(int i = 0; i < familyPlayback.size(); i++)
            {
                stopTimer(familyPlayback[i].familyID);
            }
        }
    }
    
    
    struct PlaybackParameters
    {
        EFamilyID familyID{eFamilyID_none};
        int playbackSpeedMS{100};
        
        int metricIndx{0};
        
        String familyAddress{""};
        StringArray metricStrings;
    };
    std::vector<PlaybackParameters> familyPlayback;
    
    
    OSCHandler *osc{nullptr};

};


