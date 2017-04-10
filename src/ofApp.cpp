#include "ofApp.h"


//--------------------------------------------------------------
void ofApp::setup(){
    
    //normal - change ofSetColor too
    //fg_color = ofColor(255.0, 255.0, 255.0);
    //bg_color = ofColor(0.0, 0.0, 0.0);
    
    //ofBackground(bg_color);
    
    ofSetFrameRate(60);
    
    ofPixels pix;
    pix.allocate(ofGetWidth(), ofGetHeight(), OF_PIXELS_RGBA);
    
    //scale it properly
    int w = 640;
    int h = 640;
    
    // setup the reaction-diffusion simulation
    rd.setup(w, h); // setup the simulation with the specified width and height
    
    //rd.kickstart(4); // randomly set N substance values to kickstart the simulation
    
    // setup the result image with the same dimensions as the simulation
    result.allocate(w, h, OF_IMAGE_COLOR);
    
}

//--------------------------------------------------------------
void ofApp::update(){
    
    rd.step(1); // number of simulation steps per frame
    //rd.getImage(result, fg_color, bg_color); // put the result of the simulation into the image
    rd.getImage(result);
    ofSetWindowTitle("fps: " + ofToString(ofGetFrameRate(), 0));
    
}

//--------------------------------------------------------------
void ofApp::draw(){
    //ofSetColor(255.0, 255.0, 255.0);
    result.draw(0, 0, ofGetWidth(), ofGetHeight());
}
