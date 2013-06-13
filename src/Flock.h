/*
 *  Flock.h
 *  emptyExample
 *
 *  Created by Oriol Ferrer Mesià on 24/03/12.
 *  Copyright 2012 uri.cat. All rights reserved.
 *
 */

#pragma once

#include "ofMain.h"
#include "FlockParams.h"
#include "FlockMember.h"
#include "constants.h"
#include "ofxTimeMeasurements.h"

class FlockMember;

class Flock {
	
	public:

		Flock( FlockParams* params );
		~Flock(void);
	
		void update(float dt);
		void draw();
	
		void calcDistances();

		float				getDist(int fish1, int fish2);
		FlockMember*		getMember(int ID);
		FlockParams*		getParams(){return params;}

	private:
	
		FlockParams *params;
		FlockMember* members[NUM_FISH_PER_FLOCK];

		ofMesh forcesMesh;
		ofMesh pointsMesh;
		
		float distances[NUM_FISH_PER_FLOCK][NUM_FISH_PER_FLOCK];
};

