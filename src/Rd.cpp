//
//  Rd.cpp
//  reaction-diffusion
//
//  Created by Jennifer Yarboro on 11/7/16.
//
//  Updated by Jennifer Yarboro to exclude out of date code on 01/19/17

#include "Rd.h"

void Rd::setup(int _w, int _h){
    w = _w;
    h = _h;
    vectorSize = w * h;
    setupNeighbourMap();
    setupDefaults();
    
    myFile.open(ofToDataPath("color_data.csv"), ofFile::Append, false);
}

void Rd::setupNeighbourMap(){
    for(int y = 0; y < h; y++){
        for(int x = 0; x < w; x++){
            vector <int> neighbours;
            int p = x + y * w;
            
            // left neighbour
            if(x == 0){
                //neighbours.push_back(p + (w - 1));
                neighbours.push_back(p);
            }
            else{
                neighbours.push_back(p - 1);
            }
            // right neighbour
            if(x == w - 1){
                //neighbours.push_back(p - (w - 1));
                neighbours.push_back(p);
            }
            else{
                neighbours.push_back(p + 1);
            }
            // top neighbour (this may be the bottom)
            if(y == 0){
                //neighbours.push_back(p + w * (h - 1));
                neighbours.push_back(p);
            }
            else{
                neighbours.push_back(p - w);
            }
            // bottom neighbour (this may be the top)
            if(y == h - 1){
                //neighbours.push_back(p - w * (h - 1));
                neighbours.push_back(p);
            }
            else{
                neighbours.push_back(p + w);
            }
            N.push_back(neighbours);
        }
    }
}

void Rd::setupDefaults(){
    
    //setup parameters
    Le = 1.0;
    mu = 1.8;
    epsilon = 0.18;
    kappa = 0.0005;
    
    //setup x, y coordinates
    double a_init;
    double b_init;
    B = vector <float>(vectorSize, 0.0);
    
    //initial conditions
    a_init = (mu / exp( mu / (1.0 + (mu * epsilon))));
    A = vector <float>(vectorSize, a_init);
    
    double L = 10.0;
    int N = 640;
    int index = 0;
    
    for (int y_star = 0; y_star < h; y_star++) {
        for (int x_star = 0; x_star < w; x_star++) {
            
            double x = (L * x_star) / N;
            double y = (L * y_star) / N;
            
            if (x > y) {
                b_init = mu + exp(-50.0 * (pow(x - 5.0, 2.0) + pow(y - 3.5, 2.0)));
            } else {
                b_init = mu + exp(-50.0 * (pow(x - 5.0, 2.0) + pow(y - 6.5, 2.0)));
            }
            B[index] = b_init;
            
            index++;
        }
    }
    
    //next value for diffusion
    An = vector <float>(vectorSize, 0.0);
    Bn = vector <float>(vectorSize, 0.0);

    //diffusion - source terms
    Ad = vector <float>(vectorSize, 1.0);
    Bd = vector <float>(vectorSize, 1.0);
}

void Rd::step(int numSteps){
    for(int i = 0; i < numSteps; i++){
        diffusion();
        reaction();
    }
}

void Rd::diffusion(){
    
    for(int p = 0; p < vectorSize; p++){
        const vector <int> & P = N[p];
        An[p] = A[p] + Ad[p] * ((A[P[0]] + A[P[1]] + A[P[2]] + A[P[3]] - 4 * A[p]) / 4.0);
        Bn[p] = B[p] + Bd[p] * ((B[P[0]] + B[P[1]] + B[P[2]] + B[P[3]] - 4 * B[p]) / 4.0);
    }
    // after calculating next matrix, copy it into current matrix
    A = An;
    B = Bn;
}

void Rd::reaction(){
    
    for(int p = 0; p < vectorSize; p++){
        float a;
        float b;
        //float ab2 = a * b * b;
        
        //check if p is on a boundary of box
        const vector <int> & P = N[p];
        if (P[0] == 0 || P[1] == 0 || P[2] == 0 || P[3] == 0) {
            a = (mu / exp( mu / (1 + mu * epsilon)));
            b = mu;
        } else {
            a = A[p];
            b = B[p];
        }
        
        //homogeneous state fk rates
        //most resembles a burning front
        //float feedRate = 0.034;
        //float killRate = 0.055;
        
        //A[p] = A[p] - ab2 + feedRate * (1.0f - a);
        //B[p] = B[p] + ab2 - (feedRate + killRate) * b;

        double b_pow = b / (1.0 + b * epsilon);

        A[p] = A[p] + 0.000244 * (mu - (a * exp(b_pow)));
        B[p] = Le * B[p] + 0.000244 * ((1.0 / kappa) * ((a * exp(b_pow)) - b));
    
    }
}

//void Rd::kickstart(int num){
//    for(int i = 0; i < num; i++){
//        B[int((i + 1) * (w / (num + 1)) * h)] = 1.0;
//    }
//}

//void Rd::getImage(ofImage & image, const ofColor & c1, const ofColor & c2){
void Rd::getImage(ofImage & image){
    
    ofPixels & pixels = image.getPixels();
    
    myFile << "index" << "," << "value" << "," << "fractBetween" << std::endl;
    
    for(int indexImg = 0, indexB = 0; indexB < B.size(); indexImg += 3, indexB++){
        
        //lerped means linear interpolation
        //ofColor c = c1.getLerped(c2, A[indexA] * A[indexA]);
        //does the same thing as getLerped
        //ofColor c = (c1 * B[indexB] * B[indexB]) + (c2 * (1 - (B[indexB] * B[indexB])));
        
        const int NUM_COLORS = 7;
        static double color[NUM_COLORS][3] = {
            {240,231,70},          //yellow
            {241,91,43},           //orange
            {157,40,91},           //magenta
            {39,85,166},           //dark purple
            {75,98,173},           //dark purple
            {155,161,208},         //purple blue
            {247,247,251}          //light blue
        };
        
        double value = (B[indexB]*B[indexB]) / 3.246;
        
        int idx1;
        int idx2;
        double fractBetween = 0;
        
        if(value <= 0) {
            idx1 = idx2 = 0;
        } else if(value >= 1) {
            idx1 = idx2 = NUM_COLORS-1;
        } else {
            value *= (NUM_COLORS-1);
            idx1 = floor(value);
            idx2 = idx1+1;
            fractBetween = value - double(idx1);
        }
        
        pixels[indexImg] = (color[idx2][0] - color[idx1][0]) * fractBetween + color[idx1][0];
        pixels[indexImg + 1] = (color[idx2][1] - color[idx1][1]) * fractBetween + color[idx1][1];
        pixels[indexImg + 2] = (color[idx2][2] - color[idx1][2]) * fractBetween + color[idx1][2];
        
        myFile << indexB << "," << value << "," << fractBetween << std::endl;
        
        //pixels[indexImg] = c.r;
        //pixels[indexImg + 1] = c.b;
        //pixels[indexImg + 2] = c.g;
    }
    image.update();
}























