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

class Trails{

public:

	void setup(ofxColorGradient g);

	void update(ofVec3f pos);
	void draw();

	void generateMesh();

	void calcRadius(float & rad, float percent);

	vector <ofVec3f> positions;
	ofMesh mesh;
	float radius;
	int maxLength;
	ofxColorGradient gradient;
	int circleRes;
	AnimCurve headTailcurve;
	ofMesh normals;
	int skipStep;


};
#endif /* defined(__Trails__Trails__) */
