#pragma once

#include <stdlib.h>

typedef struct {
	
	float rethinkTime;		

	float cohesionDist;
	float collisionDist;
	float schoolFriendsDist;
	float schoolOthersDist;

	float schoolFriends;
	float schoolOthersF;
	float avoidanceF;
	float cohesionF;
	float centerismF;
	
	float maxSpeedMagnitude;	//tag 10
	float accelerationF;
	
	bool debugShowCollisions;
	bool debugShowCohesion;
	bool debugShowSchooling;
	bool debugShowDirection;
	
	bool debugCohesionDist;
	bool debugCollisionDist;
	bool debugSchoolingFriendsDist;
	bool debugSchoolingOthersDist;
	
}FlockParams;
