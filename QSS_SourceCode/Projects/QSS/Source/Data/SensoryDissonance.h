#include <vector>
#include <math.h>
#include <stdio.h>
#include <iostream>
#include <list>



struct dissSpectrum {
    
	float totSpecDiss;
    std::vector<float> fundamentals;
	std::vector<float> freq;
	std::vector<float> amp;
	std::vector<float> bark;
	std::vector<double> sone;
	std::vector<std::vector<float> > diss;
	std::vector<std::vector<float> > sonWgts;

};


class SensDissonance
{
public:
    SensDissonance();
    ~SensDissonance();
    
    int testDissClass();
    float freqtoBark(float fq);
    double dBtoPhons(float fq, float dB);
    double dBtoSones(float fq, float dB);
    double PhontodB(float fq, float phon);
    float parncuttDiss(float bk1, float bk2);
    void calcDissonance(dissSpectrum &ds);
    
    void calculateEqualLoudnessContour(float phon);
    
};
