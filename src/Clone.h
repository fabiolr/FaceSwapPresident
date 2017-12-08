#pragma once

#include "ofMain.h"

class Clone {
public:
    ofFbo getBuffer();
	void setup(int width, int height);
	void setStrength(int strength);
	void update(ofTexture& src, ofTexture& dst, ofTexture& mask);
	void draw(float x, float y);
	
protected:
	void maskedBlur(ofTexture& tex, ofTexture& mask, ofFbo& result);
	ofFbo buffer, srcBlur, dstBlur, copyBuffer;
	ofShader maskBlurShader, cloneShader;
	int strength;
};
