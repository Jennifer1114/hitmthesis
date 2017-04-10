//
//  Rd.cpp
//  reaction-diffusion
//
//  Created by Jennifer Yarboro on 11/7/16.
//
//

#include "Rd.h"

void Rd::setup(int _w, int _h){
    w = _w;
    h = _h;
    vectorSize = w * h;
    setupNeighbourMap();
    setupDefaults();
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
    A = vector <float>(vectorSize, 1.0f);
    An = vector <float>(vectorSize, 1.0f);
    Ad = vector <float>(vectorSize, 0.5f);
    
    B = vector <float>(vectorSize, 0.0f);
    Bn = vector <float>(vectorSize, 0.0f);
    Bd = vector <float>(vectorSize, 0.25f);
    
    //D = vector <float>(vectorSize, 0.0f);
}

//void Rd::setFeedRates(float f0, float f1){
//    F[0] = f0;
//    F[1] = f1;
//}

//void Rd::setKillRates(float k0, float k1){
//    K[0] = k0;
//    K[1] = k1;
//}

void Rd::step(int numSteps){
    for(int i = 0; i < numSteps; i++){
        diffusion();
        reaction();
    }
}

void Rd::diffusion(){
    for(int p = 0; p < vectorSize; p++){
        const vector <int> & P = N[p];
        An[p] = A[p] + Ad[p] * ((A[P[0]] + A[P[1]] + A[P[2]] + A[P[3]] - 4 * A[p]) / 4.0f);
        Bn[p] = B[p] + Bd[p] * ((B[P[0]] + B[P[1]] + B[P[2]] + B[P[3]] - 4 * B[p]) / 4.0f);
    }
    // after calculating next matrix, copy it into current matrix
    A = An;
    B = Bn;
}

void Rd::reaction(){
    for(int p = 0; p < vectorSize; p++){
        float a = A[p];
        float b = B[p];
        float ab2 = a * b * b;
        
        // use the division rate to determine this cells feed and kill rate
        //float feedRate = D[p] * F[0] + (1 - D[p]) * F[1];
        //float killRate = D[p] * K[0] + (1 - D[p]) * K[1];
        
        //mitosis growth feed/kill rates
        float feedRate = 0.0367;
        float killRate = 0.0649;
        
        //coral growth feed/kill rates
        //float feedRate = 0.0545;
        //float killRate = 0.062;
        
        A[p] = A[p] - ab2 + feedRate * (1.0f - a);
        B[p] = B[p] + ab2 - (feedRate + killRate) * b;
    }
}

void Rd::kickstart(int num){
    for(int i = 0; i < num; i++){
        B[int((i + 1) * (w / (num + 1)) * h)] = 1.0f;
    }
}

//void Rd::setImage(ofPixels input){
//    input.resize(w, h); // resize input image to simulation dimensions
//    int numChannels = input.getNumChannels();
//    for(int i = 0; i < vectorSize; i++){
//        int a = input[i * numChannels + 3]; // get alpha of pixel
//        D[i] = a / 255.0;
//    }
//}

void Rd::getImage(ofImage & image, const ofColor & c1, const ofColor & c2){
    ofPixels & pixels = image.getPixels();
    for(int indexImg = 0, indexA = 0; indexA < A.size(); indexImg += 3, indexA++){
        ofColor c = c1.getLerped(c2, A[indexA] * A[indexA]);
        pixels[indexImg] = c.r;
        pixels[indexImg + 1] = c.b;
        pixels[indexImg + 2] = c.g;
    }
    image.update();
}
