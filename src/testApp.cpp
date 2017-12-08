#include "testApp.h"

using namespace ofxCv;
using namespace cv;

void testApp::setup() {
#ifdef TARGET_OSX
    //ofSetDataPathRoot("../data/");
#endif
    ofSetVerticalSync(true);
    cloneReady = false;
    cam.initGrabber(640, 480);
    clone.setup(cam.getWidth(), cam.getHeight());
    ofFbo::Settings settings;
    settings.width = cam.getWidth();
    settings.height = cam.getHeight();
    maskFbo.allocate(settings);
    bufferFbo.allocate(settings);
    srcFbo.allocate(settings);
    camTracker.setup();
    camTrackerCopy.setup();
    srcTracker.setup();
    multiTracker.setup(2, 640, 480);
    srcTracker.setIterations(25);
    srcTracker.setAttempts(4);
    
    faces.allowExt("jpg");
    faces.allowExt("png");
    faces.listDir("faces");
    currentFace = 0;
    if(faces.size()!=0){
        loadFace(faces.getPath(currentFace));
    }
}

void testApp::update() {
    cam.update();
    if(cam.isFrameNew()) {
        cv::Mat frame = toCv(cam);
        cv::Mat copyFrame;
        copy(frame, copyFrame);

        camTracker.update(frame);
        cloneReady = camTracker.getFound();
        if(cloneReady) {
            ofMesh camMesh = camTracker.getImageMesh();
            camMesh.clearTexCoords();
            camMesh.addTexCoords(srcPoints);
            
            cv::Rect bRect = cv::boundingRect(toCv(camTracker.getImagePoints()));
            cv::rectangle(copyFrame, bRect, cvScalar(0, 0, 0), -1, 8, 0);
//            cv::imshow("copyFrame", copyFrame);

            maskFbo.begin();
            ofClear(0, 255);
            camMesh.draw();
            maskFbo.end();

            srcFbo.begin();
            ofClear(0, 255);
            src.bind();
            camMesh.draw();
            src.unbind();
            srcFbo.end();

            clone.setStrength(16);
            clone.update(srcFbo.getTexture(), cam.getTexture(), maskFbo.getTexture());
        

            camTrackerCopy.update(copyFrame);
            if(camTrackerCopy.getFound()) {
                ofMesh camMesh = camTrackerCopy.getImageMesh();
                camMesh.clearTexCoords();
                camMesh.addTexCoords(srcPoints);

                maskFbo.begin();
                ofClear(0, 255);
                camMesh.draw();
                maskFbo.end();

                srcFbo.begin();
                ofClear(0, 255);
                src.bind();
                camMesh.draw();
                src.unbind();
                srcFbo.end();

                clone.setStrength(16);
                clone.update(srcFbo.getTexture(), clone.getBuffer().getTexture(), maskFbo.getTexture());
            }
        }
    }
}

void testApp::draw() {
    ofSetColor(255);
    
    if(src.getWidth() > 0 && cloneReady) {
        clone.draw(0, 0);
    } else {
        cam.draw(0, 0);
    }
    
    if(!camTracker.getFound()) {
        ofDrawBitmapStringHighlight("camera face not found", 10, 10);
    }
    if(src.getWidth() == 0) {
        ofDrawBitmapStringHighlight("drag an image here", 10, 30);
    } else if(!srcTracker.getFound()) {
        ofDrawBitmapStringHighlight("image face not found", 10, 30);
    }
}

void testApp::loadFace(string face){
    src.load(face);
    if(src.getWidth() > 0) {
        srcTracker.update(toCv(src));
        srcPoints = srcTracker.getImagePoints();
    }
}

void testApp::dragEvent(ofDragInfo dragInfo) {
    loadFace(dragInfo.files[0]);
}

void testApp::keyPressed(int key){
    switch(key){
        case OF_KEY_UP:
            if(currentFace < faces.size() - 1)
                currentFace++;
            break;
        case OF_KEY_DOWN:
            if(currentFace > 0)
                currentFace--;
            break;
    }
    currentFace = ofClamp(currentFace,0,faces.size());
    if(faces.size()!=0){
        loadFace(faces.getPath(currentFace));
    }
}
