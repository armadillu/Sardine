#pragma once

#include <stdlib.h>

typedef struct {
	
	float rethinkTime;		

	float cohesionDist;
	float collisionDist;
	float schoolFriendsDist;
	float schoolOthersDist;
	float centerRadius;

	float schoolFriends;
	float schoolOthersF;
	float collisionAvoidF;
	float cohesionF;
	float centerismF;
	
	float maxSpeedMagnitude;	//tag 10
	float accelerationF;
	float mediumFriction;
	
	bool debugShowCollisions;
	bool debugShowCohesion;
	bool debugShowSchooling;
	bool debugShowDirection;
	
	bool debugCohesionDist;
	bool debugCollisionDist;
	bool debugSchoolingFriendsDist;
	bool debugSchoolingOthersDist;
	
}FlockParams;
