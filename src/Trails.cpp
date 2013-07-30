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
	//mesh.setMode(OF_PRIMITIVE_LINE_STRIP);
	params = p;
	params->primitiveMode = OF_PRIMITIVE_TRIANGLES;
	points.setMode(OF_PRIMITIVE_POINTS);

	mat.setAmbientColor( g.getColorAtPercent(0) );
	mat.setDiffuseColor(g.getColorAtPercent(0) );
	mat.setSpecularColor( ofColor(255));
	mat.setShininess(25.0f);
	timeOffset = ofRandom(100);
}


void Trails::update(ofVec3f pos){

	//positions.push_back(pos);
	int ind = params->ondulationPeriod;
	if (positions.size() > ind){
		ofVec3f dir = pos - positions[ind];
		dir /= ind;
		dir.rotate( params->ondulationAmp * sin( params->ondulationFreq * ofGetElapsedTimef() + timeOffset), dir.crossed(ofVec3f(1,0,0)) );

		positions.insert(positions.begin(), positions[0] + dir );
	}else{
		positions.insert(positions.begin(), pos);
	}
	
	while(positions.size() > params->maxLength){
		positions.erase(positions.end()-1);
	}	
}


void Trails::clear(){
	positions.clear();
}


void Trails::generateMesh(){

	points.clear();
	mesh.clear();
	if(params->drawNormals){
		normals.clear();
		normals.setMode(OF_PRIMITIVE_LINES);
	}
	mesh.setMode(params->primitiveMode);
	int skip = params->skipStep;
	int n = floor(positions.size() / skip);

//	cout << "##########################################" << endl;
//	cout << "real n: " << positions.size() << "    rest: " << rest << endl;
//	cout << "##########################################" << endl;

	ofVec3f unit = ofVec3f(0.0f,-1.0f,0.0f);
	
	float pu = 1.0f / (n-1);
	for(int i = 0; i < n - 1 ; i += 1){

		float percent0 = (i) / (float)(n - 1  );
		float percent1 =  (i + 1) / (float)(n - 1);

		//cout <<"i: " << i <<"    n: " << n << "    percent0: " << percent0 << "     percent1: " << percent1 << endl;
		float r0 = params->radius ;// * percent0 + percent0 * val3 * 0.3 * radius * sinf(val2 * percent0 + val1 * ofGetElapsedTimef()) + val4 * ofNoise(ofGetElapsedTimef() * val5 * percent0);
		float r1 = params->radius ;// * percent1 + percent1 * val3 * 0.3 * radius * sinf(val2 * percent1 + val1 * ofGetElapsedTimef()) + val4 * ofNoise(ofGetElapsedTimef() * val5 * percent1);

		//handle head / tail radius
		calcRadius(r0, percent0);
		calcRadius(r1, percent1);

		//printf("r0: %f    r1: %f\n",r0,r1);
		ofVec3f p0 = positions[i * skip];
		ofVec3f p1 = positions[(i + 1) * skip];
		ofVec3f p2 = positions[(i + 2) * skip];
		ofVec3f dir0 = p1 - p0;
		ofVec3f dir1 = p2 - p1;
		ofVec3f perpDir0 = dir0.getPerpendicular( unit ).normalize();
		ofVec3f perpDir1 = dir1.getPerpendicular( unit ).normalize();

		points.addVertex(p0);

		//printf("#################\n");
		float circumf = 180;
		for(int j = 0; j < params->circleRes ; j++ ){
			float percent00 = j / (float)(params->circleRes - 1);
			float percent01 = (j+1) / (float)(params->circleRes - 1);
			float s00 = sin(2.0f * M_PI * percent00);
			float s01 = sin(2.0f * M_PI * percent01);
			float flatR00 = r0 - r0 * params->flatness * s00;
			float flatR11 = r1 - r1 * params->flatness * s01;
			float flatR01 = r0 - r0 * params->flatness * s01;
			float flatR10 = r1 - r1 * params->flatness * s00;

			ofVec3f v00 = (perpDir0).getRotated(circumf * percent00, dir0);
			ofVec3f v11 = (perpDir1).getRotated(circumf * percent01, dir1);
			ofVec3f v01 = (perpDir0).getRotated(circumf * percent01, dir0);
			ofVec3f v10 = (perpDir1).getRotated(circumf * percent00, dir1);

			if(params->drawNormals){
				float normalAng = (perpDir1 * (r1-r0)).angle(dir0);
				normals.addVertex(p0);
				normals.addVertex(p0 + v00.getRotated(normalAng, dir0.getPerpendicular(perpDir0)) * (r0 + 5.0f) );
			}
			if(addColorDataToMesh)mesh.addColor(gradient.getColorAtPercent(percent0));
			//mesh.addColor(ofColor::blue);
			mesh.addNormal( v00 );
			mesh.addVertex( p0 + flatR00 * v00  );

			if(addColorDataToMesh)mesh.addColor(gradient.getColorAtPercent(percent0));
			//mesh.addColor(ofColor::green);
			mesh.addNormal(v01);
			mesh.addVertex( p0 + flatR01 * v01 );

			if(addColorDataToMesh)mesh.addColor(gradient.getColorAtPercent(percent1));
			//mesh.addColor(ofColor::red);
			mesh.addNormal( v11 );
			mesh.addVertex( p1 + flatR11 * v11 );


			
			if(addColorDataToMesh)mesh.addColor(gradient.getColorAtPercent(percent0));
			//mesh.addColor(ofColor::cyan);
			mesh.addNormal(v00);
			mesh.addVertex( p0 + flatR00 * v00 );

			if(addColorDataToMesh)mesh.addColor(gradient.getColorAtPercent(percent1));
			//mesh.addColor(ofColor::orange);
			mesh.addNormal(v10);
			mesh.addVertex( p1 + flatR10 * v10 );
			
			if(addColorDataToMesh)mesh.addColor(gradient.getColorAtPercent(percent1));
			//mesh.addColor(ofColor::magenta);
			mesh.addNormal(v11);
			mesh.addVertex( p1 + flatR11 * v11 );
		}
	}
}


void Trails::calcRadius(float & rad, float percent){

	float head = params->headLen;
	float tail = params->tailLen;

	if (percent < head){ // head
		float pp = percent / head;
		rad *= (ofxAnimatable::calcCurveAt(pp, params->headTailcurve));
	}else{ // tail

		if( percent > 1.0f - tail ){
			float pp = (percent - (1.0 - tail)) / tail;
			rad *= ( ofxAnimatable::calcCurveAt( 1 - pp, params->headTailcurve));
		}
	}
}

void Trails::draw(){

	//ofNoFill();
	//points.draw();
	mat.begin();
	mesh.draw();
	mat.end();

	if(params->drawNormals){
		ofEnableBlendMode(OF_BLENDMODE_ALPHA);
		ofSetColor(2550,0,0,255);
		glDisable(GL_LIGHTING);
		normals.draw();
		glEnable(GL_LIGHTING);
	}

}

