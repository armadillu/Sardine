//
//  Trails.cpp
//  Trails
//
//  Created by Oriol Ferrer Mesià on 09/06/13.
//
//

#include "Trails.h"

void Trails::setup(ofxColorGradient g, TrailParams * p){

	gradient = g;
	mesh.setMode(OF_PRIMITIVE_TRIANGLES);
	//mesh.setMode(OF_PRIMITIVE_LINE_STRIP);
	params = p;
}


void Trails::update(ofVec3f pos){

	positions.push_back(pos);
	
	while(positions.size() > params->maxLength){
		positions.erase(positions.begin());
	}
	generateMesh();
}


void Trails::generateMesh(){

	mesh.clear();
	normals.clear();
	normals.setMode(OF_PRIMITIVE_LINES);
	int skip = params->skipStep;
	int rest = (positions.size() - 1)%skip;
	int n = positions.size() - rest;
	

	if ( n < 2 * skip + 1 ) return;

//	cout << "##########################################" << endl;
//	cout << "real n: " << positions.size() << "    rest: " << rest << endl;
//	cout << "##########################################" << endl;

	for(int i = n - 1; i >= skip ; i -= skip){

		float percent0 = (i ) / (float)(n - 1);
		float percent1 = (i - skip) / (float)(n - 1);

		//cout <<"i: " << i <<" n: " << n << "    percent0: " << percent0 << "     percent1: " << percent1 << endl;
		float r0 = params->radius * percent0;// * percent0 + percent0 * val3 * 0.3 * radius * sinf(val2 * percent0 + val1 * ofGetElapsedTimef()) + val4 * ofNoise(ofGetElapsedTimef() * val5 * percent0);
		float r1 = params->radius * percent1;// * percent1 + percent1 * val3 * 0.3 * radius * sinf(val2 * percent1 + val1 * ofGetElapsedTimef()) + val4 * ofNoise(ofGetElapsedTimef() * val5 * percent1);

		//handle head / tail radius
		calcRadius(r0, percent0);
		calcRadius(r1, percent1);

		ofVec3f p0 = positions[i];
		ofVec3f p1 = positions[i-skip];
		ofVec3f p2 = positions[i-2*skip];
		ofVec3f dir0 = p1 - p0;
		ofVec3f dir1 = p2 - p1;
		ofVec3f perpDir0 = dir0.getPerpendicular( ofVec3f(1,1,1) ).normalize();
		ofVec3f perpDir1 = dir1.getPerpendicular( ofVec3f(1,1,1) ).normalize();

		float circumf = 360;
		for(int j = 0; j < params->circleRes ; j++ ){
			float percent00 = j / (float)(params->circleRes);
			float percent01 = (j+1) / (float)(params->circleRes);

			ofVec3f v00 = (perpDir0).getRotated(circumf * percent00, dir0);
			ofVec3f v11 = (perpDir1).getRotated(circumf * percent01, dir1);
			ofVec3f v01 = (perpDir0).getRotated(circumf * percent01, dir0);
			ofVec3f v10 = (perpDir1).getRotated(circumf * percent00, dir1);
			//normals.addVertex(p0);
			//normals.addVertex(p0 + v00 * (radius + 4) );
			mesh.addColor(gradient.getColorAtPercent(percent0));
			mesh.addNormal( v00 );
			mesh.addVertex( p0 + r0 * v00  );
			mesh.addColor(gradient.getColorAtPercent(percent1));
			mesh.addNormal( v11 );
			mesh.addVertex( p1 + r1 * v11 );
			mesh.addColor(gradient.getColorAtPercent(percent0));
			mesh.addNormal(v01);
			mesh.addVertex( p0 + r0 * v01 );

			mesh.addColor(gradient.getColorAtPercent(percent0));
			mesh.addNormal(v00);
			mesh.addVertex( p0 + r0 * v00 );
			mesh.addColor(gradient.getColorAtPercent(percent1));
			mesh.addNormal(v10);
			mesh.addVertex( p1 + r1 * v10 );
			mesh.addColor(gradient.getColorAtPercent(percent1));
			mesh.addNormal(v11);
			mesh.addVertex( p1 + r1 * v11 );
			
		}
	}
}

void Trails::calcRadius(float & rad, float percent){

	float head = 0.1;
	float tail = 0.1;

	if (percent < head){
		float pp = percent / head;
		rad *= ofxAnimatable::calcCurveAt(pp, params->headTailcurve);
	}else{
		if( percent > 1.0f - tail ){
			float pp = (percent - (1.0 - tail)) / tail;
			rad *= ofxAnimatable::calcCurveAt(1.0f - pp, params->headTailcurve);
		}
	}
}

void Trails::draw(){

	ofNoFill();
	mesh.draw();
	ofEnableBlendMode(OF_BLENDMODE_ADD);
	ofSetColor(255,128);
	//normals.draw();

}

