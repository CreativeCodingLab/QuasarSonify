#include "SensoryDissonance.h"

SensDissonance::SensDissonance(){
    testDissClass();
}

SensDissonance::~SensDissonance(){
}

int SensDissonance::testDissClass()
{
    std::cout << "Sensory Dissonance Class is hooked up and ready!!!!" << std::endl;

    return 0;
}

//******************************************************************
// Convert frequencies to barks
//******************************************************************
float SensDissonance::freqtoBark(float fq){	
    //	float fq = 440;
	float bk = 0;
	
	bk = ((26.81*fq)/(1960.0+fq)) - 0.53;
	if (bk < 2.0) 
		bk += 0.15*(2.0-bk);
	else if (bk > 20.1) 
		bk += 0.22*(bk - 20.1);
	else  
		bk = bk;
    
	return bk;
}


double SensDissonance::dBtoPhons(float fq, float dB)
{
    /*    Below are the equations that returns the sound pressure level (dB) for each freq and phon level (Ln)
     The equation is revised to solve for (Ln), thus returning the Phon instead of spl (dB).
     eq1: Af=4.47E-3 * (10.^(0.025*Ln) - 1.15) + (0.4*10.^(((Tf+Lu)/10)-9 )).^af;
     eq2: Lp=((10./af).*log10(Af)) - Lu + 94;
     
     NOTE: we wil break the above formula up into chunks:
     chunk1  is derived from eq2, only we solve for "Af"
     chunk2 = is from eq1 -> (0.4*10.^(((Tf+Lu)/10)-9 )).^af
     Phon = is derived from eq1, only we solve for "Ln" and relabel it as "Phon".
     */
    
    double freq[29] = {20, 25, 31.5, 40, 50, 63, 80, 100, 125, 160, 200, 250, 315, 400, 500, 630, 800,
        1000, 1250, 1600, 2000, 2500, 3150, 4000, 5000, 6300, 8000, 10000, 12500};
    
    double af[29] = {0.532, 0.506, 0.480, 0.455, 0.432, 0.409, 0.387, 0.367, 0.349, 0.330, 0.315,
        0.301, 0.288, 0.276, 0.267, 0.259, 0.253, 0.250, 0.246, 0.244, 0.243, 0.243,
        0.243, 0.242, 0.242, 0.245, 0.254, 0.271, 0.301};
    
    double Lu[29] = {-31.6, -27.2, -23.0, -19.1, -15.9, -13.0, -10.3, -8.1, -6.2, -4.5, -3.1,
        -2.0, -1.1, -0.4, 0.0, 0.3, 0.5, 0.0, -2.7, -4.1, -1.0, 1.7, 2.5,
        1.2, -2.1, -7.1, -11.2, -10.7, -3.1};
    
    double Tf[29] = {78.5, 68.7, 59.5, 51.1, 44.0, 37.5, 31.5, 26.5, 22.1, 17.9, 14.4,
        11.4, 8.6, 6.2, 4.4, 3.0, 2.2, 2.4, 3.5, 1.7, -1.3, -4.2,
        -6.0, -5.4, -1.5, 6.0, 12.6, 13.9, 12.3};
    
    
    //determine which frequency range we are in
    int fqRange[2];
    for(int i = 0; i < 29; i++){
        if (fq <= freq[i]){
            fqRange[0] = i-1;
            fqRange[1] = i;
            break;
        }
    }
    
    //Uses ISO226 values of equal contours for calculating phons
    double Phon[2];
    for(int i = 0; i < 2; i++){
        double chunk1 = pow(10, 0.1*af[fqRange[i]]*(dB + Lu[fqRange[i]] - 94));
        double chunk2 = pow(0.4 * pow(10, ((Tf[fqRange[i]]+Lu[fqRange[i]])/10)-9 ), af[fqRange[i]]);
        Phon[i] = 40*log10(211.41649*(chunk1 - chunk2) + 1.15);
    }
    
    //linear interpolation between phones
    double iPhon = Phon[0] + ((fq - freq[fqRange[0]])/(freq[fqRange[1]] - freq[fqRange[0]]))*(Phon[1] - Phon[0]);
    
    
    return iPhon;
}



//******************************************************************************
// Convert sound pressure level to sones via equal loudness contours from ISO226
//******************************************************************************
double SensDissonance::dBtoSones(float fq, float dB)
{
	//linear interpolation between phones
    double iPhon = dBtoSones(fq, dB);
	
	//Calculate sone based on linearly interpolated phon
	return pow(2, (iPhon - 40)/10);
}


double SensDissonance::PhontodB(float fq, float phon)
{
    /*    Below are the equations that returns the sound pressure level (dB) for each freq and phon level (Ln)
     The equation is revised to solve for (Ln), thus returning the Phon instead of spl (dB).
     eq1: Af=4.47E-3 * (10.^(0.025*Ln) - 1.15) + (0.4*10.^(((Tf+Lu)/10)-9 )).^af;
     eq2: Lp=((10./af).*log10(Af)) - Lu + 94;
     
     NOTE: we wil break the above formula up into chunks:
     chunk1  is derived from eq2, only we solve for "Af"
     chunk2 = is from eq1 -> (0.4*10.^(((Tf+Lu)/10)-9 )).^af
     Phon = is derived from eq1, only we solve for "Ln" and relabel it as "Phon".
     */
    
    double freq[29] = {20, 25, 31.5, 40, 50, 63, 80, 100, 125, 160, 200, 250, 315, 400, 500, 630, 800,
        1000, 1250, 1600, 2000, 2500, 3150, 4000, 5000, 6300, 8000, 10000, 12500};
    
    double af[29] = {0.532, 0.506, 0.480, 0.455, 0.432, 0.409, 0.387, 0.367, 0.349, 0.330, 0.315,
        0.301, 0.288, 0.276, 0.267, 0.259, 0.253, 0.250, 0.246, 0.244, 0.243, 0.243,
        0.243, 0.242, 0.242, 0.245, 0.254, 0.271, 0.301};
    
    double Lu[29] = {-31.6, -27.2, -23.0, -19.1, -15.9, -13.0, -10.3, -8.1, -6.2, -4.5, -3.1,
        -2.0, -1.1, -0.4, 0.0, 0.3, 0.5, 0.0, -2.7, -4.1, -1.0, 1.7, 2.5,
        1.2, -2.1, -7.1, -11.2, -10.7, -3.1};
    
    double Tf[29] = {78.5, 68.7, 59.5, 51.1, 44.0, 37.5, 31.5, 26.5, 22.1, 17.9, 14.4,
        11.4, 8.6, 6.2, 4.4, 3.0, 2.2, 2.4, 3.5, 1.7, -1.3, -4.2,
        -6.0, -5.4, -1.5, 6.0, 12.6, 13.9, 12.3};
    
    
    //determine which frequency range we are in
    int fqRange[2];
    for(int i = 0; i < 29; i++)
    {
        if (fq <= freq[i])
        {
            fqRange[0] = i-1;
            fqRange[1] = i;
            break;
        }
    }
    
    
//        %Error Trapping
//        if((phon < 0) | (phon > 90))
//            disp('Phon value out of bounds!')
//            spl = 0;
//        freq = 0;
//        else
//            %Setup user-defined values for equation
//                Ln = phon;
    
    float Ln = phon;
    
       //Deriving sound pressure level from loudness level (iso226 sect 4.1)
       // Af=4.47E-3 * (10.^(0.025*Ln) - 1.15) + (0.4*10.^(((Tf[indx]+Lu[indx])/10)-9 )).^af[indx];
    //        Lp=((10./af).*log10(Af)) - Lu + 94;

    float dB[2] = {0,0};
    for(int i = 0; i < 2; i++)
    {
        int indx = fqRange[i];
        
        
        // 4.47E-3 * (10.^(0.025*Ln) - 1.15)
        float chunk1 = 0.00447*(powf(10, 0.025*Ln) - 1.15);
        // ( 0.4*10.^( ( (Tf+Lu) /10 )-9 ) ).^af
        float chunk2 = powf(0.4*powf(10.,((Tf[indx]+Lu[indx])/10.)-9.), af[indx]);
        float Af = chunk1 + chunk2;
        
        // ((10./af).*log10(Af)) - Lu + 94
        dB[i] = (10./af[indx])*log10(Af) - Lu[indx] + 94.;
    }
    
    float dB_interp = dB[0] + ((fq - freq[fqRange[0]])/(freq[fqRange[1]] - freq[fqRange[0]]))*(dB[1] - dB[0]);
    
 //
//    //linear interpolation between phones
//    double iPhon = Phon[0] + ((fq - freq[fqRange[0]])/(freq[fqRange[1]] - freq[fqRange[0]]))*(Phon[1] - Phon[0]);
    
    return dB_interp;
}


//******************************************************************
// Dissonance Equation (from Parncutt)
//******************************************************************
float SensDissonance::parncuttDiss(float bk1, float bk2){	
	float diss = 0;
    //	float bk1 = 21.4;
    //	float bk2 = 22;
	
	float bkintrvl = fabs(bk1 - bk2); 
	if (bkintrvl<1.2) 
		diss=pow(exp(1)*(4*bkintrvl)*exp(-(4.0*bkintrvl)),2); 
	else diss=0;
    
	return diss;
}


//********************************************************************************
// Calculate the dissonance present in the spectrum
//*********************************************************************************
void SensDissonance::calcDissonance(dissSpectrum &ds){	
	//NEEDS: ADD ROUTINE FOR MASKING
	
    
	//Calculate Bark Dissonance
	std::vector<float> d;
	for(int i = 0; i < ds.freq.size(); i++){
		for(int j = i; j < ds.freq.size(); j++){
            float calcDiss = parncuttDiss(ds.bark[i], ds.bark[j]);
			d.push_back(calcDiss);
		}
 		ds.diss.push_back(d);
		d.clear();
	}
	
    //TEST PRINT**************
    //	cout << "PRINT OUT OF Dissonance" << endl;
    //	for(int i = 0; i < ds.diss.size(); i++){
    //		cout << "Diss at " << i << ": ";
    //		for(int j = 0; j < ds.diss[i].size(); j++){
    //			cout << ds.diss[i].at(j) << "; ";
    //		}
    //		cout << endl;
    //	}
    //	return;	
    ////************************
	
	
	//Combine sones for each dissonance combination, total, and create soneWeight.
	float totSone= 0;
	for(int i = 0; i < ds.freq.size(); i++){
		for(int j = i; j < ds.freq.size(); j++){
			d.push_back(ds.sone[i] + ds.sone[j]);
			totSone += ds.sone[i] + ds.sone[j];
		}
		ds.sonWgts.push_back(d);
		d.clear();
	}
	for(int i = 0 ; i < ds.sonWgts.size(); i++){
		for(int j = 0; j < ds.sonWgts[i].size(); j++){
			ds.sonWgts[i].at(j) = (totSone == 0) ? 0: ds.sonWgts[i].at(j)/totSone;
		}
	}
 	
	//Finally calculate total dissonance as weighted average (based on sonWgts) of dissonances betweend dyads.
	ds.totSpecDiss = 0;
	for(int i = 0 ; i < ds.diss.size(); i++){
		for(int j = 0; j < ds.diss[i].size(); j++){
			ds.totSpecDiss += ds.sonWgts[i].at(j)*ds.diss[i].at(j);
		}
	}
	
    
}



void SensDissonance::calculateEqualLoudnessContour(float phon)
{
    // This method should loop through all the data points and calculate an equal loudness contour for the desired phon level.
    // code below taken from: https://www.mathworks.com/matlabcentral/fileexchange/7028-iso-226-equal-loudness-level-contour-signal
    
    
    //        %                /---------------------------------------\
    //        %%%%%%%%%%%%%%%%%          TABLES FROM ISO226             %%%%%%%%%%%%%%%%%
    //        %                \---------------------------------------/
    //        f = [20 25 31.5 40 50 63 80 100 125 160 200 250 315 400 500 630 800 ...
    //             1000 1250 1600 2000 2500 3150 4000 5000 6300 8000 10000 12500];
    //        af = [0.532 0.506 0.480 0.455 0.432 0.409 0.387 0.367 0.349 0.330 0.315 ...
    //              0.301 0.288 0.276 0.267 0.259 0.253 0.250 0.246 0.244 0.243 0.243 ...
    //              0.243 0.242 0.242 0.245 0.254 0.271 0.301];
    //        Lu = [-31.6 -27.2 -23.0 -19.1 -15.9 -13.0 -10.3 -8.1 -6.2 -4.5 -3.1 ...
    //              -2.0  -1.1  -0.4   0.0   0.3   0.5   0.0 -2.7 -4.1 -1.0  1.7 ...
    //              2.5   1.2  -2.1  -7.1 -11.2 -10.7  -3.1];
    //        Tf = [ 78.5  68.7  59.5  51.1  44.0  37.5  31.5  26.5  22.1  17.9  14.4 ...
    //              11.4   8.6   6.2   4.4   3.0   2.2   2.4   3.5   1.7  -1.3  -4.2 ...
    //              -6.0  -5.4  -1.5   6.0  12.6  13.9  12.3];
    //        %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
    //        %Error Trapping
    //        if((phon < 0) | (phon > 90))
    //            disp('Phon value out of bounds!')
    //            spl = 0;
    //        freq = 0;
    //        else
    //            %Setup user-defined values for equation
    //                Ln = phon;
    //        %Deriving sound pressure level from loudness level (iso226 sect 4.1)
    //        Af=4.47E-3 * (10.^(0.025*Ln) - 1.15) + (0.4*10.^(((Tf+Lu)/10)-9 )).^af;
    //        Lp=((10./af).*log10(Af)) - Lu + 94;
    //        %Return user data
    //        spl = Lp;
    //        freq = f;
    //        end
    
}


