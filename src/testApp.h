#pragma once

#include "ofMain.h"
#include "ofxCv.h"
#include "Clone.h"
//#include "ofxFaceTracker.h"
//#include "ofxFaceTrackerThreaded.h"
#include "ofxFaceTrackerMulti.h"
using namespace ofxCv;
using namespace cv;

#include "ofxFaceTracker.h"

class testApp : public ofBaseApp {
public:
    void setup();
    void update();
    void draw();
    void dragEvent(ofDragInfo dragInfo);
    void loadFace(string face);
    
    void keyPressed(int key);
    
    ofxFaceTrackerMulti multiTracker;
    ofxFaceTrackerThreaded camTracker;
    ofxFaceTrackerThreaded camTrackerCopy;
    ofVideoGrabber cam;
    
    ofxFaceTracker srcTracker;
    ofImage src;
    vector<ofVec2f> srcPoints;
    
    bool cloneReady;
    Clone clone;
    ofFbo srcFbo, maskFbo, bufferFbo;
    
    ofDirectory faces;
    int currentFace;
};
