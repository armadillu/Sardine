//
//  Trails.h
//  Trails
//
//  Created by Oriol Ferrer Mesià on 09/06/13.
//
//

#ifndef __Trails__Trails__
#define __Trails__Trails__

#include <iostream>

#include "ofMain.h"
#include "ofxAnimatable.h"
#include "ofxColorGradient.h"

struct TrailParams{
	float radius;
	int circleRes;
	int maxLength;
	int skipStep;
	float headLen;
	float tailLen;
	AnimCurve headTailcurve;
	ofPrimitiveMode primitiveMode;
	bool drawNormals;
	float flatness;
	
};

class Trails{

public:



	void setup(ofxColorGradient g, TrailParams * p);

	void update(ofVec3f pos);
	void draw();
	void clear();

	void generateMesh();

	void calcRadius(float & rad, float percent);

	vector <ofVec3f> positions;
	ofMesh mesh;
	ofxColorGradient gradient;

	ofMesh normals;
	ofMesh points;
	TrailParams * params;
};
#endif /* defined(__Trails__Trails__) */
