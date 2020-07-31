/*
  ==============================================================================

    This file was auto-generated!

  ==============================================================================
*/

#include "DataHandler.h"
#include "../MainComponent.h"

//==============================================================================

DataHandler::DataHandler() : Thread ("Load Audio Thread")
{
}


void DataHandler::loadData(bool loadInThread)
{
    if(loadInThread)
    {
        startThread(0);
    }
    else
    {
        File agentDataFile(RESOURCE_PATH + "/TON580_norm.ascii");
        retrieveRawData(agentDataFile);
    }
}


void DataHandler::run()
{
    // this is the code that runs this thread - we'll loop continuously,
    // updating the coordinates of our blob.
    
    // threadShouldExit() returns true when the stopThread() method has been
    // called, so we should check it often, and exit as soon as it gets flagged.
    while (! threadShouldExit())
    {
        // sleep a bit so the threads don't all grind the CPU to a halt..
        //wait (interval);
        
        // because this is a background thread, we mustn't do any UI work without
        // first grabbing a MessageManagerLock..
        const MessageManagerLock mml (Thread::getCurrentThread());
        
        if (! mml.lockWasGained())  // if something is trying to kill this job, the lock
            return;                 // will fail, in which case we'd better return..
        
        //            // now we've got the UI thread locked, we can mess about with the components
        
        
        File agentDataFile(RESOURCE_PATH + "/TON580_norm.ascii");
        retrieveRawData(agentDataFile);
        
        printRawData();
                
        signalThreadShouldExit();
    }
}


String DataHandler::selectDataFileToLoad()
{
    String message = "Select Data File to Load...";
    String fileExtension = "";
    String fileFilter = "*" + fileExtension;
    
    
    FileChooser dataFileChooser (message, File::getSpecialLocation (File::userDesktopDirectory), fileFilter);
    
    if(dataFileChooser.browseForFileToOpen())
    {
        File dataFile = dataFileChooser.getResult();
        
        retrieveRawData(dataFile);
        
        std::cout << "data file loaded: " << dataFile.getFileNameWithoutExtension() << std::endl;
        
        return dataFile.getFileName();
    }

    return "";
}


void DataHandler::retrieveRawData(File dataFile)
{
    // Raw Wavelengh vs flux dataset...
    
    if (! dataFile.existsAsFile())
    {
        std::cout << "file does not exist" << std::endl;
        return;
    }
    std::cout << "----- found data file for import -----" << std::endl;
    
    
    FileInputStream inputStream (dataFile);
    if (! inputStream.openedOk())
    {
        std::cout << "could not open Input stream for data" << std::endl;
        return;
    }
    
    loadedDataFile = dataFile;
    
    
    String asterix ("*");
    int count = 0;
    
    while (! inputStream.isExhausted()) // [3]
    {
        String line = inputStream.readNextLine();
        
        if(line.startsWith("WAVE"))
        {
            // skip the line...
        }
        else
        {
            StringArray sArray;
            sArray.addTokens(line, ",", "");
            RawData rd;
            
            rd.wavelength = sArray[0].getFloatValue();
            rd.flux = sArray[1].getFloatValue();
            rd.error = sArray[2].getFloatValue();
            
            rawWavelengthData.push_back(rd);
        }
        
        if(count % 10000 == 0){
            std::cout << "count is at: " << count << std::endl;
         }
        count++;
        //std::cout << line << std::endl;
    }
    
    
    std::cout << "------- Done importing agent data ----------" << std::endl;
    
    String doneReporting;
    doneReporting << "------------------------------------" << newLine
    << "------------------------------------" << newLine
    << "------- Done importing agent data ----------" << newLine
    << "------------------------------------" << newLine
    << "------------------------------------" << newLine;
//    MAIN->mainView->reportComponent.statusReport.setText(doneReporting);
    
}

void DataHandler::loadAtomicDataList()
{
    File atomicDataFile(RESOURCE_PATH + "/verner6.txt");

    
    if (! atomicDataFile.existsAsFile())
    {
        std::cout << "file does not exist" << std::endl;
        return;
    }
    std::cout << "----- found data file for import -----" << std::endl;
    
    
    FileInputStream inputStream (atomicDataFile);
    if (! inputStream.openedOk())
    {
        std::cout << "could not open Input stream for data" << std::endl;
        return;
    }
    
    
    String asterix ("*");
    int count = 0;
    
    while (! inputStream.isExhausted()) // [3]
    {
        String line = inputStream.readNextLine();
        
        AtomicTransitionProperties atProperties;
        atProperties.name = line.substring(10, 18).trimEnd();
        atProperties.restframeRate = line.substring(0, 11).getDoubleValue();
        atProperties.absorptionProbability = line.substring(40, 45).getDoubleValue();
        
        // translate the oscillator strength to a double
        String oscStrengthString = line.substring(30, 39);
        double leadingCoeff = oscStrengthString.substring(0, 5).getDoubleValue();
        String plusMinusSign = oscStrengthString.substring(6, 7);
        double exponent = oscStrengthString.substring(7, 9).getDoubleValue();
        exponent = plusMinusSign.containsIgnoreCase("+") ? exponent : -exponent;
        atProperties.oscillatorStrength = leadingCoeff * pow(10, exponent);
        
        
        // filter on the data entries, only allow those within the followin thresholds...
        if(atProperties.absorptionProbability >= 9.75
           && atProperties.restframeRate >= 900
           && atProperties.restframeRate <= 1800)
        {
            //std::cout << "----> "<< leadingCoeff << ": " << plusMinusSign << ": " << exponent << std::endl;
            //std::cout << "name: " << atProperties.name << ", restFrame: " << atProperties.restframeRate << ", oscStrngth: " << atProperties.oscillatorStrength << ", prob: " << atProperties.absorptionProbability << std::endl;
        
            atomicTransitions.push_back(atProperties);
        }
    }
    

    
    std::vector<String> namesAlreadyPlaced;
    
    for(int i = 0; i < atomicTransitions.size(); i++)
    {
        String checkName(atomicTransitions[i].name);
        
        
        // check to see if we have already placed the name...
        bool nameIsPlaced = false;
        for(int j = 0; j < namesAlreadyPlaced.size(); j++)
        {
            if(checkName == namesAlreadyPlaced[j])
            {
                nameIsPlaced = true;
            }
        }
        if(nameIsPlaced)
        {
            continue;
        }
        
        
        // name hasn't been placed, so add all elements with names to a new array.
        namesAlreadyPlaced.push_back(checkName);
        std::vector<SpectralData> sdVec;
        for(int j = i; j < atomicTransitions.size(); j++)
        {
            if(checkName == atomicTransitions[j].name)
            {
                SpectralData sd;
                sd.label = atomicTransitions[j].name;
                sd.restFrameRate = atomicTransitions[j].restframeRate;
                sd.oscillatorStrength = atomicTransitions[j].oscillatorStrength;
                sd.absorptionProbability = atomicTransitions[j].absorptionProbability;
                sd.displayName = sd.label.removeCharacters(" ") + " " + String((int)sd.restFrameRate);
                sdVec.push_back(sd);
            }
            
        }
        
        
        // lambda to sort by absorption probability (descending)
        std::sort(sdVec.begin(), sdVec.end(),
                  [](SpectralData const & a, SpectralData const & b) -> bool
                  { return a.absorptionProbability > b.absorptionProbability; } );
        
        
        // add the array to the entire set of arrays.
        atomSpectraList.push_back(sdVec);
    }
    
    
    // filter the element list to keep only the top 3 absorption probabilities for each element.
    for(int i = 0; i < atomSpectraList.size(); i++)
    {
        for(int j = 0; j < atomSpectraList[i].size(); j++)
        {
            // CII
            if(atomSpectraList[i][0].label == "C II")
            {
                if(!(atomSpectraList[i][j].absorptionProbability == 11.04
                   || atomSpectraList[i][j].absorptionProbability == 10.79))
                {
                    atomSpectraList[i].erase(atomSpectraList[i].begin() + j);
                    j--;
                }
            }
            // H I
            if(atomSpectraList[i][0].label == "H I")
            {
                if(!(atomSpectraList[i][j].absorptionProbability == 14.53
                     || atomSpectraList[i][j].absorptionProbability == 13.73
                     || atomSpectraList[i][j].absorptionProbability == 13.45))
                {
                    atomSpectraList[i].erase(atomSpectraList[i].begin() + j);
                    j--;
                }
            }
            // C I
            if(atomSpectraList[i][0].label == "C I")
            {
                if(!(atomSpectraList[i][j].absorptionProbability == 10.97
                     || atomSpectraList[i][j].absorptionProbability == 10.93
                     || atomSpectraList[i][j].absorptionProbability == 10.66))
                {
                    atomSpectraList[i].erase(atomSpectraList[i].begin() + j);
                    j--;
                }
            }
            // C I*
            if(atomSpectraList[i][0].label == "C I*")
            {
                if(!(atomSpectraList[i][j].absorptionProbability == 10.97
                     || atomSpectraList[i][j].absorptionProbability == 10.55
                     || (atomSpectraList[i][j].absorptionProbability == 10.53 && j == 2)))
                {
                    atomSpectraList[i].erase(atomSpectraList[i].begin() + j);
                    j--;
                }
            }
            // C I**
            if(atomSpectraList[i][0].label == "C I**")
            {
                if(!(atomSpectraList[i][j].absorptionProbability == 10.97
                     || atomSpectraList[i][j].absorptionProbability == 10.8
                     || (atomSpectraList[i][j].absorptionProbability == 10.58 && j == 2)))
                {
                    atomSpectraList[i].erase(atomSpectraList[i].begin() + j);
                    j--;
                }
            }
            // O I
            if(atomSpectraList[i][0].label == "O I")
            {
                if(!(atomSpectraList[i][j].absorptionProbability == 10.75
                     || atomSpectraList[i][j].absorptionProbability == 10.58
                     || atomSpectraList[i][j].absorptionProbability == 10.14))
                {
                    atomSpectraList[i].erase(atomSpectraList[i].begin() + j);
                    j--;
                }
            }
            // O I*
            if(atomSpectraList[i][0].label == "O I*")
            {
                if(!(atomSpectraList[i][j].absorptionProbability == 10.75
                     || atomSpectraList[i][j].absorptionProbability == 10.51
                     || atomSpectraList[i][j].absorptionProbability == 10.12))
                {
                    atomSpectraList[i].erase(atomSpectraList[i].begin() + j);
                    j--;
                }
            }
            // O I**
            if(atomSpectraList[i][0].label == "O I**")
            {
                if(!(atomSpectraList[i][j].absorptionProbability == 10.75
                     || atomSpectraList[i][j].absorptionProbability == 10.64
                     || atomSpectraList[i][j].absorptionProbability == 10.24))
                {
                    atomSpectraList[i].erase(atomSpectraList[i].begin() + j);
                    j--;
                }
            }
            // SiII
            if(atomSpectraList[i][0].label == "SiII")
            {
                if(!(atomSpectraList[i][j].absorptionProbability == 10.72
                     || atomSpectraList[i][j].absorptionProbability == 10.39
                     || atomSpectraList[i][j].absorptionProbability == 9.85))
                {
                    atomSpectraList[i].erase(atomSpectraList[i].begin() + j);
                    j--;
                }
            }
            // SiI
            if(atomSpectraList[i][0].label == "SiI")
            {
                if(!(atomSpectraList[i][j].absorptionProbability == 10.32
                     || atomSpectraList[i][j].absorptionProbability == 10.11
                     || atomSpectraList[i][j].absorptionProbability == 10.07))
                {
                    atomSpectraList[i].erase(atomSpectraList[i].begin() + j);
                    j--;
                }
            }
        }
    }
    
    
    // set the default frequencies for sonification
    for(int i = 0; i < atomSpectraList.size(); i++)
    {
        std::cout << "--- new element: " << std::endl;
        for(int j = 0; j < atomSpectraList[i].size(); j++)
        {
            if(j == 0)
            {
                atomSpectraList[i][j].centralFrequency = 440/2;
            }
            else if(j == 1)
            {
                atomSpectraList[i][j].centralFrequency = 440/2 * 1.5;
            }
            else if(j == 2)
            {
                atomSpectraList[i][j].centralFrequency = 440/2 * 1.25;
            }
        }
    }
    

    // print out the organized list...
//    std::cout << "------------- Printing the Organized Transition Data: " << atomicTransitions.size() << std::endl;
//    for(int i = 0; i < atomSpectraList.size(); i++)
//    {
//        std::cout << "--- new element: " << std::endl;
//        for(int j = 0; j < atomSpectraList[i].size(); j++)
//        {
//            std::cout << j << ": name: " << atomSpectraList[i][j].displayName << ", restRate: " << atomSpectraList[i][j].restFrameRate << ", oscStrength: " << atomSpectraList[i][j].oscillatorStrength << ", prob: " << atomSpectraList[i][j].absorptionProbability << std::endl;
//        }
//    }
    
    
}



void DataHandler::writeFilteredDataToFile()
{
    File outFile("~/Desktop/filteredAgentData.txt");
    
//    // currenlty filtering  the last time step...
//    int start = allAgentData.size() - 1;
//    for(int i = start; i < allAgentData.size(); i++)
//    {
//        outFile.appendText("*** timestep " + String(i) + "\n");
//
//        std::cout << "-------- new time step " << i << " -----" << std::endl;
//        for(int j = 0; j < allAgentData[i].size(); j++)
//        {
//
//            String data( String(allAgentData[i][j].position.x) + ", " + String(allAgentData[i][j].position.y) + ", " + String(allAgentData[i][j].position.z) + ", " + String(allAgentData[i][j].velocity.x) + ", " + String(allAgentData[i][j].velocity.y) + ", " + String(allAgentData[i][j].velocity.z) + ", " + String(allAgentData[i][j].density) + "\n" );
//
//            outFile.appendText(data);
//        }
//    }

}


String DataHandler::printRawData()
{
    std::cout << "------------------------------------" << std::endl;
    std::cout << "---------- printing ALL agent data -----------" << std::endl;
    std::cout << "------------------------------------" << std::endl;

    for(int i = 0; i < rawWavelengthData.size(); i++)
    {
        std::cout << rawWavelengthData[i].wavelength << ", " << rawWavelengthData[i].flux << ", " << rawWavelengthData[i].error << std::endl;
    }
    
    return "";
}


String DataHandler::printFilteredData()
{
    
//    std::sort(filteredAgentData.begin(), filteredAgentData.end(), compareDensity);
//
//
//    String agentDataString;
//    agentDataString
//    << "---------------------------------------------------" << newLine
//    << "---------- printing FILTERED agent data -----------" << newLine
//    << "----------" << newLine
//    << "----- Number of agents: " << filteredAgentData.size() << newLine
//    << "----------" << newLine;
//
//    for(int i = 0; i < filteredAgentData.size(); i++)
//    {
//        agentDataString
//        << "Agnt " << i + 1 << " pos: " << filteredAgentData[i].position.x << ", " << filteredAgentData[i].position.y << ", " << filteredAgentData[i].position.z << " ..." << "Dnsty: " << filteredAgentData[i].density << newLine;//", Normalized Density: " << filteredAgentData[i].density_Normalized << "freq = " << filteredAgentData[i].frequency << ", pan = " << filteredAgentData[i].pan << ", amp = " << filteredAgentData[i].amplitude << std::endl;
//    }
//
//    return agentDataString;
}




// --------------------------------------------------------
// ---------------- custom data handling methods
// --------------------------------------------------------


void DataHandler::setSpectralDataForAnalysis(int indx)
{
    
    for(int i = 0; i < atomSpectraList.size(); i++)
    {
        if(i == indx)
        {
            selectedSpectralData = atomSpectraList[i];
            break;
        }
    }
    
    
    // set min/max values for red shift and velocity ranges.
    setMinMaxLymanRedShift(selectedSpectralData);
    setMinMaxLymanVelocities(selectedSpectralData);
}



void DataHandler::convertToHydrogenLymanVelocities()
{
    float lymAlpha = 1215.67;
    float lymBeta = 1025.72;
    float lymGamma = 972.54;
    
    float redShift = DEFAULT_REDSHIFT;
    
    for(int i = 0; i < rawWavelengthData.size(); i++)
    {
        float redShiftWaveLength = rawWavelengthData[i].wavelength / (1 + redShift);
        
        float hVelocity_alpha = (redShiftWaveLength - lymAlpha) / lymAlpha * SPEEDOFLIGHT_C;
        float hVelocity_beta = (redShiftWaveLength - lymBeta) / lymBeta * SPEEDOFLIGHT_C;
        float hVelocity_gamma = (redShiftWaveLength - lymGamma) / lymGamma * SPEEDOFLIGHT_C;
    }
}


void DataHandler::getFluxFromRedShift(std::vector<SpectralData> &specData, float redShift)
{
    // (1 + redshift) * lymanX = observed wavelength that we want….
    
    
    // setup flags for finding a red shift match and set the red shift...
    std::vector<bool> foundVelMatch;
    for(int i = 0; i < specData.size(); i++)
    {
        foundVelMatch.push_back(false);
        specData[i].redShift = redShift;
    }

    
    // find the fulx for each spectral transition.
    for(int i = 0; i < rawWavelengthData.size() - 1; i++)
    {
        float dataWavelength_l = rawWavelengthData[i].wavelength;
        float dataWavelength_h = rawWavelengthData[i+1].wavelength;
        
        for(int j = 0; j < specData.size(); j++)
        {
            float redShiftWavelength = (1 + redShift) * specData[j].restFrameRate;
            
            
            
            if(redShiftWavelength >= dataWavelength_l && redShiftWavelength <= dataWavelength_h)
            {
                //std::cout << "------------- FOUND MATCH ---------------" << std::endl;
                //std::cout << dataWavelength_l << ", " << redShiftWavelength << ", " << dataWavelength_h << std::endl;

                float interpAmnt = (redShiftWavelength - dataWavelength_l) / (dataWavelength_h - dataWavelength_l);
                float interpFlux = rawWavelengthData[i].flux + interpAmnt*(rawWavelengthData[i+1].flux - rawWavelengthData[i].flux);
                specData[j].flux = interpFlux;
                
                float interpWavelength = rawWavelengthData[i].wavelength + interpAmnt*(rawWavelengthData[i+1].wavelength - rawWavelengthData[i].wavelength);
                specData[j].velocity = computeVelocityFromWavelength(interpWavelength, specData[j].restFrameRate, redShift);
                
                
                // compute the width of the spectral transition.
                boundaryAtHalfMaximum(i, specData[j]);
                
                foundVelMatch[j] = true;
            }
        }
    }
    
}


void DataHandler::getFluxFromVelocity(std::vector<SpectralData> &specData, float velocity, float redShift)
{
    //redShift = 0.20257;
    
    // setup flags for finding a velocity match...
    std::vector<bool> foundVelMatch;
    for(int i = 0; i < specData.size(); i++)
    {
        specData[i].velocity = velocity;
        foundVelMatch.push_back(false);
    }
    //    std::cout << "------------- new velocity ------------" << std::endl;
    // look for the corresponding velocity and get the flux...
    for(int i = 0; i < rawWavelengthData.size() - 1; i++)
    {
        float redShiftWaveLength_l = rawWavelengthData[i].wavelength / (1 + redShift);
        float redShiftWaveLength_h = rawWavelengthData[i+1].wavelength / (1 + redShift);
        
        
        for(int j = 0; j < specData.size(); j++)
        {
            if(foundVelMatch[j]) continue;
            
            
            float lymanValue = specData[j].restFrameRate;
            float velocity = specData[j].velocity;
            
            float hVelocity_alpha_l = (redShiftWaveLength_l - lymanValue) / lymanValue * SPEEDOFLIGHT_C;
            float hVelocity_alpha_h = (redShiftWaveLength_h - lymanValue) / lymanValue * SPEEDOFLIGHT_C;
            
            if(velocity >= hVelocity_alpha_l && velocity <= hVelocity_alpha_h)
            {
                float interpAmnt = (velocity - hVelocity_alpha_l) / (hVelocity_alpha_h - hVelocity_alpha_l);
                float interpFlux = rawWavelengthData[i].flux + interpAmnt*(rawWavelengthData[i+1].flux - rawWavelengthData[i].flux);
                specData[j].flux = interpFlux;
                
                float interpWavelength = rawWavelengthData[i].wavelength + interpAmnt*(rawWavelengthData[i+1].wavelength - rawWavelengthData[i].wavelength);
                redShiftatVelocity = interpWavelength / lymanValue - 1;
                specData[j].redShift = interpWavelength / lymanValue - 1;
                
                std::cout << "----------------- red shift value: " << redShiftatVelocity << std::endl;
                std::cout << "-------- lyman value: " << lymanValue << std::endl;
                std::cout << "-------- flux value: " << interpFlux << std::endl;
                
                
                // compute the width of the spectral transition
                boundaryAtHalfMaximum(i, specData[j]);
                
                
                // compute the acd... ACD = 1 / (oscStrength * restWavelength) * ln(1 / observedWavelengthFlux)
                specData[j].ApparentColumnDensity = 1. / (specData[j].oscillatorStrength * specData[j].restFrameRate) * log(1. / interpFlux);
                
                
                foundVelMatch[j] = true;
                specData[j].rawDataIndex = i;
            }
        }
    }
}


float DataHandler::computeVelocityFromWavelength(float wavelength, float lymanX, float redShift)
{
    float redShiftWavelength = wavelength / (1 + redShift);
    
    return (redShiftWavelength - lymanX) / lymanX * SPEEDOFLIGHT_C;
}


float DataHandler::setMinMaxLymanVelocities(std::vector<SpectralData> &specData)
{
    int maxIndx = rawWavelengthData.size() - 1;
    
    const int dataSize = specData.size();
    
    minMaxVelocity_Absolute[0] = 99999;
    minMaxVelocity_Absolute[1] = -99999;

    for(int i = 0; i < dataSize; i++)
    {
        float lymanValue = specData[i].restFrameRate;
        float redShiftWavelength = rawWavelengthData[0].wavelength / (1 + specData[i].redShift);
        specData[i].minVelocity = (redShiftWavelength - lymanValue) / lymanValue * SPEEDOFLIGHT_C;
        
        redShiftWavelength = rawWavelengthData[maxIndx].wavelength / (1 + specData[i].redShift);
        specData[i].maxVelocity = (redShiftWavelength - lymanValue) / lymanValue * SPEEDOFLIGHT_C;
        
        
        if(specData[i].minVelocity < minMaxVelocity_Absolute[0])
        {
            minMaxVelocity_Absolute[0] = specData[i].minVelocity;
        }
        
        if(specData[i].maxVelocity > minMaxVelocity_Absolute[1])
        {
            minMaxVelocity_Absolute[1] = specData[i].maxVelocity;
        }

    }

    return 0.;
}


float DataHandler::setMinMaxLymanRedShift(std::vector<SpectralData> &specData)
{
    // (1 + redshift) * lymanX = observed wavelength that we want….
    // redshift = wavelength / lymanX - 1;
    
    const int maxIndx = rawWavelengthData.size() - 1;
    const int dataSize = specData.size();
    
    minMaxRedShift_Absolute[0] = 99999;
    minMaxRedShift_Absolute[1] = -99999;
    
    for(int i = 0; i < dataSize; i++)
    {
        float lymanValue = specData[i].restFrameRate;
        
        specData[i].minRedShift = rawWavelengthData[0].wavelength / lymanValue - 1;
        specData[i].maxRedShift = rawWavelengthData[maxIndx].wavelength / lymanValue - 1;
        
        if(specData[i].minRedShift < minMaxRedShift_Absolute[0])
        {
            minMaxRedShift_Absolute[0] = specData[i].minRedShift;
        }
        
        if(specData[i].maxRedShift > minMaxRedShift_Absolute[1])
        {
            minMaxRedShift_Absolute[1] = specData[i].maxRedShift;
        }
    }

    return 0.;
}


float DataHandler::boundaryAtHalfMaximum(int startIndx, SpectralData &specData)
{
    float flux_l = 0;
    float flux_h = 0;
    
    float redShift = 0.20257; //specData.redShift;
    
    
    //NOTE: flux basically ranges from 0 to 1, and hover around 1 when there is little absorption. Our half max measurement is the distance between the local min and 1, so half max = 1 - 0.5*localMinDistance.
    float localMinDistance = 1 - specData.flux;
    float targetHalfMaxFlux = 1 - 0.5 * localMinDistance;
    float lymanValue = specData.restFrameRate;
    
    // find least upper bound.
    bool found = false;
    int currentIndx = startIndx;
    while(!found && currentIndx < rawWavelengthData.size() - 1)
    {
        flux_l = rawWavelengthData[currentIndx].flux;
        flux_h = rawWavelengthData[currentIndx + 1].flux;
        
        if(targetHalfMaxFlux < flux_l)
        {
            if(targetHalfMaxFlux > flux_h){
                found = true;
            }
        }
        else if(targetHalfMaxFlux > flux_l)
        {
            if(targetHalfMaxFlux < flux_h){
                found = true;
            }
        }
        
        currentIndx++;
    }
    
    // find the interpolated wavelength where the target flux is.
    float interpAmnt = (targetHalfMaxFlux - flux_l) / (flux_h - flux_l);
    float interpWavelength = rawWavelengthData[currentIndx].wavelength + interpAmnt*(rawWavelengthData[currentIndx+1].wavelength - rawWavelengthData[currentIndx].wavelength);
    
    //convert the wavelength to velocity.
    float redShiftWaveLength = interpWavelength / (1 + redShift);
    specData.velocityHalfMax_lub = (redShiftWaveLength - lymanValue) / lymanValue * SPEEDOFLIGHT_C;
    
    specData.redShiftHalfMax_lub = interpWavelength / lymanValue - 1;
    
    
    // reset the index, and find greatest lower bound.
    found = false;
    currentIndx = startIndx;
    while(!found && currentIndx >= 0)
    {
        flux_l = rawWavelengthData[currentIndx].flux;
        flux_h =rawWavelengthData[currentIndx + 1].flux;
        
        if(targetHalfMaxFlux < flux_l)
        {
            if(targetHalfMaxFlux > flux_h){
                found = true;
            }
        }
        else if(targetHalfMaxFlux > flux_l)
        {
            if(targetHalfMaxFlux < flux_h){
                found = true;
            }
        }
        
        currentIndx--;
    }
    
    // find the interpolated wavelength where the target flux is.
    interpAmnt = (targetHalfMaxFlux - flux_l) / (flux_h - flux_l);
    interpWavelength = rawWavelengthData[currentIndx].wavelength + interpAmnt*(rawWavelengthData[currentIndx+1].wavelength - rawWavelengthData[currentIndx].wavelength);
    
    //convert the wavelength to velocity.
    redShiftWaveLength = interpWavelength / (1 + redShift);
    specData.velocityHalfMax_glb = (redShiftWaveLength - lymanValue) / lymanValue * SPEEDOFLIGHT_C;
    
    specData.redShiftHalfMax_glb = interpWavelength / lymanValue - 1;
}


float DataHandler::computeRedShiftfromVelocity(float rawWavelength, float lymanValue)
{
    return rawWavelength / lymanValue - 1;
}



// -----------------------------------------
// ------------ Sonification methods...
// -----------------------------------------

void DataHandler::setSonificationSpectralAmplitudes(std::vector<SpectralData> &data)
{
    //float velocity = this->selectedSpectralData[0].velocity;
    float velocity = 0.0;
    float levelVal = 0.0;
    bool exceededMaxAlpha = false;
    bool exceededMaxBeta = false;
    
    
    // clear out the amplitude...
    for(int i = 0; i < data.size(); i++)
    {
        data[0].mainAmpLevel = 0;
    }
    
    
    // alpha level...
    velocity = data[0].redShift;
    if(velocity < data[0].minRedShift)
    {
        levelVal = 0;
        data[0].mainAmpLevel = 0;
    }
    else if(velocity <= data[0].maxRedShift)
    {
        levelVal = 1 - data[0].flux;
        if(levelVal > 1) levelVal = 1;
        if(levelVal < 0) levelVal = 0;
        data[0].mainAmpLevel = levelVal * levelVal;
    }
    else
    {
        levelVal = 0;
        data[0].mainAmpLevel = 0;
        
        exceededMaxAlpha = true;
    }
    //        std::cout << "---- alpha level: " << levelVal << std::endl;
    
    if(data.size() < 2)
    {
        return;
    }
    
    // beta level...
    velocity = data[1].redShift;
    if(velocity < data[1].minRedShift)
    {
        levelVal = 0;
        data[1].mainAmpLevel = 0;
    }
    else if(velocity <= data[1].maxRedShift)
    {
        levelVal = 1 - data[1].flux; //fluxValues[1];
        if(levelVal > 1) levelVal = 1;
        if(levelVal < 0) levelVal = 0;
        
        if(exceededMaxAlpha)
        {
            data[0].mainAmpLevel = levelVal * levelVal;
            data[1].mainAmpLevel = 0;
        }
        else
        {
            data[1].mainAmpLevel = levelVal * levelVal;
        }
    }
    else
    {
        levelVal = 0;
        data[1].mainAmpLevel = 0;
        
        exceededMaxBeta = true;
    }
    //        std::cout << "beta level: " << levelVal << std::endl;
    
    if(data.size() < 3)
    {
        return;
    }

    
    // lyman gamma level...
    velocity = data[2].redShift;
    if(velocity < data[2].minRedShift)
    {
        levelVal = 0;
        data[2].mainAmpLevel = 0;
    }
    else if(velocity <= data[2].maxRedShift)
    {
        levelVal = 1 - data[2].flux; //fluxValues[2];
        if(levelVal > 1) levelVal = 1;
        if(levelVal < 0) levelVal = 0;
        
        if(exceededMaxAlpha && !exceededMaxBeta)
        {
            data[1].mainAmpLevel = levelVal * levelVal;
            data[2].mainAmpLevel = 0;
        }
        else if(exceededMaxAlpha && exceededMaxBeta)
        {
            data[0].mainAmpLevel = levelVal * levelVal;
            data[2].mainAmpLevel = 0;
        }
        else
        {
            data[2].mainAmpLevel = levelVal * levelVal;
        }
    }
    else
    {
        data[2].mainAmpLevel = 0;
    }
//    std::cout << "gamma level: " << levelVal << std::endl;
    
}


void DataHandler::setSonificationBandwidthParameters(std::vector<SpectralData> &data)
{
    float RSDistanceScalar = 0.00025;
    
    for(int i = 0; i < data.size(); i++)
    {
        float totalDistance_RS = (data[i].redShiftHalfMax_lub - data[i].redShiftHalfMax_glb);
        float lowerDistance_RS = (data[i].redShift - data[i].redShiftHalfMax_glb);
        float upperDistance_RS = (data[i].redShiftHalfMax_lub - data[i].redShift);
        float centralFreq = data[i].centralFrequency;
        
        float octaveScalar = totalDistance_RS / RSDistanceScalar * 2;
        data[i].bandwidthFreq_lb = centralFreq / powf(octaveScalar, lowerDistance_RS / totalDistance_RS);
        data[i].bandwidthFreq_ub = centralFreq * powf(octaveScalar, upperDistance_RS / totalDistance_RS);
        
        float skewFactor = fabs(data[i].velocity - data[i].velocityHalfMax_glb) / fabs(data[i].velocityHalfMax_lub - data[i].velocity);
        data[i].skewFactor = skewFactor;
        data[i].skewFrequency = skewFactor * centralFreq;
    }
    
}


void DataHandler::setACDFrequency(std::vector<SpectralData> &data)
{
    
//    std::cout << "------------ acd printout" << std::endl;
    
    // need to measure deviation of ACD among spectral transitions...
    for(int i = 0; i < data.size(); i++)
    {
        float acdFreq = data[i].centralFrequency;
        
//        std::cout << "acd " << i << ": " <<  data[i].ApparentColumnDensity <<  std::endl;
        
        if(i > 0 && data[i - 1].ApparentColumnDensity != 0)
        {
            float dev = data[i].ApparentColumnDensity / data[i - 1].ApparentColumnDensity - 1;
            float pitchDev = powf(2, dev * 2 / 12);
            acdFreq = acdFreq * pitchDev;
        }
        
        data[i].acdFrequency = acdFreq;
        
//        std::cout << "acd FREQ" << i << ": " <<  data[i].acdFrequency << ", centralFreq: " << data[i].centralFrequency << std::endl;
    }
}






