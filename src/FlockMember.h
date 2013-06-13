//
//  FlockMember.h
//  Trails
//
//  Created by Oriol Ferrer Mesià on 12/06/13.
//
//

#ifndef __Trails__FlockMember__
#define __Trails__FlockMember__

#include <iostream>
#include "ofMain.h"
#include "FlockParams.h"

class Flock;

class FlockMember{

public:

	~FlockMember();
	FlockMember(ofVec3f position, ofVec3f size, ofVec3f direction,
				int ID, ofColor c, Flock* mySchool, string clan_);

	void setupWithRandomParams();

	void draw(ofMesh & pointsMesh, ofMesh &forcesMesh); //fill in the meshes
	void update(float dt);
	void advance(float dt);

	//forces calc
	void calcAllForces();

	Flock * flock;
	FlockParams * params;
	int memberID;
	bool enabled;


	float friction;
	ofVec3f pos;
	ofVec3f vel;
	ofVec3f acc;

	ofColor c;
	string clan;

	float currentSpeedMagnitude;
	float cruisingSpeed;						//this fish's cruising speed
	float turbo;								//todo!
	float maxTurbo;

	double counter;
	double thinkTimer;
	double sprintTimer;
	double maxSprintTimer;


	//forces
	ofVec3f schoolSameColor;
	ofVec3f schoolDifferentColor;

	ofVec3f avoidNeigbhors;
	ofVec3f groupCohesion;
	ofVec3f group;
	ofVec3f stayInCenter;

	ofVec3f predatorAvoidance; //?

	ofVec3f tempPoint;

};

#endif /* defined(__Trails__FlockMember__) */
