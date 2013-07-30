/*
 *  Flock.cpp
 *  emptyExample
 *
 *  Created by Oriol Ferrer Mesià on 24/03/12.
 *  Copyright 2012 uri.cat. All rights reserved.
 *
 */

#include "Flock.h"

#include "Flock.h"
#include "unistd.h"


Flock::Flock( FlockParams* params_ ){
	
	params = params_;
	ofColor colA = ofFloatColor( ofRandom(1), ofRandom(1), ofRandom(1) );
	ofColor colB = ofFloatColor( ofRandom(1), ofRandom(1), ofRandom(1) );
	
	for (int i=0; i< NUM_FISH_PER_FLOCK; i++){
		
		int rr = 100;
		string rgb;
		for(int i = 0; i < 3; i++){
			if(ofRandom(1) > 0.5){
				rgb += "1";
			}else{
				rgb += "0";
			}
		}
		ofColor cc;
		if (rgb[0] == '0') cc.r = 64;
		else cc.r = 255;
		if (rgb[1] == '0') cc.g = 64;
		else cc.g = 255;
		if (rgb[2] == '0') cc.b = 64;
		else cc.b = 255;
		float v = 500;
		members[i] = new FlockMember(	ofVec3f( ofRandom(-rr, rr), ofRandom(-rr, rr), ofRandom(-rr, rr) ),
									ofVec3f(ofRandom(-v,v), ofRandom(-v,v), ofRandom(-v,v) ),
									ofVec3f( ofRandom(-0.5, 0.5), ofRandom(-0.5, 0.5), ofRandom(-0.5, 0.5) ),
									i, 
									cc,
									this,
									rgb
						   );		
	}	
	
		
	//empty dist table
	for (int i=0; i< NUM_FISH_PER_FLOCK; i++)
		for (int j=0; j< NUM_FISH_PER_FLOCK; j++)
			distances[i][j] = 0.0f;
	
	forcesMesh.setMode(OF_PRIMITIVE_LINES);
	pointsMesh.setMode(OF_PRIMITIVE_POINTS);

}

void Flock::setup(){

}

Flock::~Flock(){}

void Flock::resetPositions(){
	float r = 5;
	for (int i=0; i< NUM_FISH_PER_FLOCK; i++){
		members[i]->pos = ofVec3f( ofRandom(-r,r), ofRandom(-r,r), ofRandom(-r,r));
	}

}

void Flock::update(float dt_){

	calcDistances();

	for (int i=0; i< NUM_FISH_PER_FLOCK; i++){
		members[i]->update(dt_);
	}	
}


void Flock::draw(){

	pointsMesh.clear();
	forcesMesh.clear();


	for (int i=0; i< NUM_FISH_PER_FLOCK; i++){
		members[i]->draw(pointsMesh, forcesMesh );
	}

	//pointsMesh.draw();
	ofSetLineWidth(1.5);
	ofDisableLighting();
	forcesMesh.draw();
	ofEnableLighting();
}



///distances are not sqr'd; we ^2 radius
void Flock::calcDistances(){

	for (int i=0; i< NUM_FISH_PER_FLOCK; i++){
		for (int j=i+1; j< NUM_FISH_PER_FLOCK; j++){
			distances[i][j] = members[i]->pos.squareDistance( members[j]->pos );
		}
	}
}


FlockMember* Flock::getMember(int ID){ 
	return members[ID];	
}


float Flock::getDist(int fish1, int fish2){
		
	if ( fish1 < fish2 )
		return distances[fish1][fish2];
	else
		return distances[fish2][fish1];
	
//	return members[fish1]->pos.distance( members[fish2]->pos );

}
