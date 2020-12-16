
#include "OverviewPlayback.h"




void OverviewPlayback::setMainOverviewPlayback(std::vector<std::vector<SpectralData> > &selectedBatchSpectralData)
{
    familyPlayback.clear();
    
    for(int i = 0; i < selectedBatchSpectralData.size(); i++)
    {
        std::vector<SpectralData> &specData = selectedBatchSpectralData[i];
        
        switch (specData[0].familyID)
        {
            case eFamilyID_1: // this is for hydrogen...
                setOverviewPlayback_Family1(specData);
                break;

            case eFamilyID_2: // whatever the other elements are...
                //int hereWeGoAgain = 0;
                //setOverviewPlayback_Family1(); hydrogen...
                break;


            default:
                break;
        }
    }
    
    
    
    // print out some playback parameters...
//    for(int i = 0; i < familyPlayback.size(); i++)
//    {
//        std::cout << "------ playback family: " << familyPlayback[i].familyID << std::endl;
//        std::cout << "------ playback address: " << familyPlayback[i].familyAddress << std::endl;
//
//        for(int j = 0; j < familyPlayback[i].metricStrings.size(); j++)
//        {
//            std::cout << "     ---- metric string: " << familyPlayback[i].metricStrings[j] << std::endl;
//        }
//    }

}


void updatePlaybackParametrsDuringPlayback(PlaybackParameters &curPlaybackParameters)
{
    if(curPlaybackParameters.familyID == eFamilyID_1)
    {
        
    }
}



void OverviewPlayback::setOverviewPlayback_Family1(std::vector<SpectralData> &specData)
{
    PlaybackParameters playPrm;
    
    // set the address
    playPrm.familyID = specData[0].familyID;
    playPrm.familyAddress = "/data/family_" + String(specData[0].familyID) + "/metrics";
    
                    
    // set the sonic metrics
    for(int i = 0; i < specData.size(); i++)
    {
        Random r;
        
        float pitch = 0;
        float attack = 0;
        float release = 0;
        float pan = 0;
        
        if(playPrm.familyID == eFamilyID_1) // Hydrogen...
        {
            if(i == 0)
            {
                pitch = 36;
            }
            else if(i == 1)
            {
                pitch = 43;
            }
            else
            {
                pitch = 52;
            }

            attack = 2000;
            release = 2000;
            
            pan = r.nextFloat();
        }
        
        
        PlaybackParameters::NoteParameters sp;
        sp.pitch = pitch;
        sp.amp = 1 - specData[i].flux;
        sp.attack = attack;
        sp.decay = 0;
        sp.sustain = 1;
        sp.release = release;
        sp.pan = pan;
        playPrm.noteList.push_back(sp);
        
        
        // Broadcasting synth parameters:
        // pitch
        // amp
        // attack
        // decay
        // sustain amp
        // release
        // stereo pan
        String metricsString = String(sp.pitch)
                               + " " + String(sp.amp)
                               + " " + String(sp.attack)
                               + " " + String(sp.decay)
                               + " " + String(sp.sustain)
                               + " " + String(sp.release)
                               + " " + String(sp.pan);
        
        playPrm.metricStrings.add(metricsString);
    }

    
    familyPlayback.push_back(playPrm);
}




void OverviewPlayback::noteListAdjustments(int noteIndx, PlaybackParameters &famPlayback)
{
    Random r;
    
    
    // hydrogen family: we randomize the stereo pan for each note...
    if(famPlayback.familyID == eFamilyID_1)
    {
        famPlayback.noteList[noteIndx].pan = r.nextFloat();
        
        
        String mString = String(famPlayback.noteList[noteIndx].pitch)
                         + " " + String(famPlayback.noteList[noteIndx].amp)
                         + " " + String(famPlayback.noteList[noteIndx].attack)
                         + " " + String(famPlayback.noteList[noteIndx].decay)
                         + " " + String(famPlayback.noteList[noteIndx].sustain)
                         + " " + String(famPlayback.noteList[noteIndx].release)
                         + " " + String(famPlayback.noteList[noteIndx].pan);

        famPlayback.metricStrings.set(noteIndx, mString);
    }
    else if(famPlayback.familyID == eFamilyID_2)
    {
        
    }
    

}

