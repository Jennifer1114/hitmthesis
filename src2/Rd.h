//
//  Rd.h
//  reaction-diffusion
//
//  Created by Jennifer Yarboro on 11/7/16.
//
//

#ifndef RD_H
#define RD_H

#include "ofMain.h"

class Rd {
    
public:
    void setup(int w, int h);
    void setupNeighbourMap();
    void setupDefaults();
    void setFeedRates(float f0, float f1);
    void setKillRates(float k0, float k1);
    void step(int numSteps);
    void diffusion();
    void reaction();
    void kickstart(int num);
    //void setImage(ofPixels input);
    void getImage(ofImage & image, const ofColor & c1, const ofColor & c2);
    
    int w, h;                   //width and height 2d dimensions
    int vectorSize;             //size for all vectors is w * h
    vector <float> A, An, Ad;   //substance A (value, next value, diffusion rate)
    vector <float> B, Bn, Bd;   //substance B
    vector <vector <int> > N;   //neighbor references for diffusion
    //float F[2];                 //feed rates
    //float K[2];                 //kill rates
    //vector <float> D;           //division rates
    
    };


#endif /* RD_H */
