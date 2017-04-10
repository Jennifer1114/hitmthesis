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
    void step(int numSteps);
    void diffusion();
    void reaction();
    void kickstart(int num);
    //void getImage(ofImage & image, const ofColor & c1, const ofColor & c2);
    void getImage(ofImage & image);

    
    int w, h;                   //width and height 2d dimensions
    int vectorSize;             //size for all vectors is w * h
    vector <float> A, An, Ad;   //substance A (value, next value, diffusion rate)
    vector <float> B, Bn, Bd;   //substance B
    vector <vector <int> > N;   //neighbor references for diffusion
    
    //simulation parameters
    double Le;
    double mu;
    double epsilon;
    double kappa;
    
    //file io
    ofFile myFile;
    
    };


#endif /* RD_H */
