#include "ofApp.h"


//--------------------------------------------------------------
void ofApp::setup(){
    fg_color = ofColor(0);
    bg_color = ofColor(255);
    
    //fbo_color = ofColor(0);
    
    ofBackground(bg_color);
    
    //dont need this cause it does background clearing after every frame
    //ofSetBackgroundAuto(false);
    //dont need this cause it does "smoothing"
    //ofEnableAntiAliasing();
    
    ofSetFrameRate(60);
    
    ofPixels pix;
    pix.allocate(ofGetWidth(), ofGetHeight(), OF_PIXELS_RGBA);
    
    //ofFbo fbo;
    //fbo.allocate(ofGetWidth(), ofGetHeight(), GL_RGBA);
    //fbo.begin();
    //ofClear(0, 0, 0, 0);
    //ofSetColor(fbo_color);
    //fbo.end();
    //ofTrueTypeFont ttf;
    //ttf.loadFont(OF_TTF_SANS, 350);
    //string s = "TYPE";
    // Center string code from:
    // https://github.com/armadillu/ofxCenteredTrueTypeFont/blob/master/src/ofxCenteredTrueTypeFont.h
    //ofRectangle r = ttf.getStringBoundingBox(s, 0, 0);
    //ofVec2f offset = ofVec2f(floor(-r.x - r.width * 0.5f), floor(-r.y - r.height * 0.5f));
    
    //ttf.drawString(s, fbo.getWidth() / 2 + offset.x, fbo.getHeight() / 2 + offset.y);
    
    //fbo.readToPixels(pix); // get the fbo's ofPixels object (to set the division rates in the RD class)
    
    int w = 320;
    int h = 180;
    
    // setup the reaction-diffusion simulation
    rd.setup(w, h); // setup the simulation with the specified width and height
    
    //rd.setFeedRates(0.0374, 0.0585); // set the in & out feed rates (in this sketch also dynamically changed in the update() loop)
    //rd.setKillRates(0.0695, 0.0610); // set the in & out kill rates
    
    rd.kickstart(4); // randomly set N substance values to kickstart the simulation
    
    //rd.setImage(pix); // use the ofPixels object of the offscreen fbo to set the division rates
    // setup the result image with the same dimensions as the simulation
    
    result.allocate(w, h, OF_IMAGE_COLOR);
    
}

//--------------------------------------------------------------
void ofApp::update(){
    // update the settings in the reaction-diffusion simulation based on the mouse position
    //float innerFeedValue = 0.0367;
    //float outerFeedValue = 0.0367;
    //rd.setFeedRates(innerFeedValue, outerFeedValue);
    
    rd.step(40); // number of simulation steps per frame
    rd.getImage(result, fg_color, bg_color); // put the result of the simulation into the image
    ofSetWindowTitle("fps: " + ofToString(ofGetFrameRate(), 0));
    
}

//--------------------------------------------------------------
void ofApp::draw(){
    ofSetColor(255);
    result.draw(0, 0, ofGetWidth(), ofGetHeight());
}
