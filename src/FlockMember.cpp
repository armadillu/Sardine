//
//  FlockMember.cpp
//  Trails
//
//  Created by Oriol Ferrer Mesià on 12/06/13.
//
//

#include "FlockMember.h"
#include "Flock.h"

FlockMember::FlockMember( ofVec3f position, ofVec3f velocity, ofVec3f direction,
						 int ID, ofColor myColor, Flock* myFlock, string clan ){

	enabled = true;
	c = myColor;
	flock = myFlock;
	pos = position;
	vel = velocity;
	params = flock->getParams();
	memberID = ID;
	this->clan = clan;
	setupWithRandomParams();
}

void FlockMember::setupWithRandomParams(){

	cruisingSpeed =  ofRandom(1, 2);
	maxTurbo = ofRandom( 0.75, 1.25);
	turbo = ofRandom(2 * maxTurbo);
	maxSprintTimer = ofRandom( 1.75, 3.25);
	sprintTimer = ofRandom(maxSprintTimer);
	thinkTimer = ofRandom(params->rethinkTime);

}


void FlockMember::advance(float dt){

	vel = vel + acc * dt ;

	currentSpeedMagnitude = vel.length();

	if (currentSpeedMagnitude > params->maxSpeedMagnitude * cruisingSpeed * ( 1.0 + turbo ) ){

		vel = vel * (params->maxSpeedMagnitude * cruisingSpeed * ( 1.0 + turbo ) / currentSpeedMagnitude );
		currentSpeedMagnitude = ( ( 1.0 + turbo ) * params->maxSpeedMagnitude * cruisingSpeed );
	}

	vel *= params->mediumFriction;
	pos = pos + vel * dt;
}


void FlockMember::update(float dt){

	if (!enabled)
		return;

	counter += dt;
	thinkTimer += dt;
	sprintTimer -= dt;
	turbo *= 0.9075;

	if ( thinkTimer > params->rethinkTime ){
		thinkTimer = params->rethinkTime;

		calcAllForces();
		if (pos.length() > params->centerRadius){
			stayInCenter = -pos;
		}else{
			stayInCenter.x = stayInCenter.y = stayInCenter.z = 0.0f;
		}


		schoolSameColor = schoolSameColor.limit(1);
		schoolDifferentColor =schoolDifferentColor.limit(1);
		//avoidNeigbhors = avoidNeigbhors.limit(1);
		groupCohesion = groupCohesion.limit(1);
		stayInCenter = stayInCenter.limit(1);

		acc =
		schoolSameColor * params->schoolFriends
		+
		schoolDifferentColor * params->schoolOthersF
		+
		avoidNeigbhors * params->collisionAvoidF
		+
		groupCohesion * params->cohesionF
		+
		stayInCenter * params->centerismF
		;
		
		//acc.normalize();
		acc = acc * params->accelerationF * 50.0f * (1.0 + turbo) ;
	}

	advance(dt);
}


void FlockMember::calcAllForces(){

	//reset all forces
	schoolSameColor.x = schoolSameColor.y = schoolSameColor.z = 0.0f;
	schoolDifferentColor.x = schoolDifferentColor.y = schoolDifferentColor.z = 0.0f;
	avoidNeigbhors.x = avoidNeigbhors.y = avoidNeigbhors.z = 0.0f;
	groupCohesion.x = groupCohesion.y = groupCohesion.z = 0.0f;

	float dist;	//non SQRT'd!
	int i;

	for( i=0; i < NUM_FISH_PER_FLOCK ; i++){

		if ( i != memberID ){

			FlockMember * mate = flock->getMember(i);
			dist = flock->getDist(memberID, i);

			float dSquared = params->collisionDist * params->collisionDist;

			if ( dist < dSquared ){	//SQUARE bc distance is not SQRT'd
				float weight = (dSquared - dist) / dSquared;
				tempPoint = ( pos - mate->pos ) * (weight);
				avoidNeigbhors = avoidNeigbhors + tempPoint;
				if ( dist < dSquared * 0.25f && sprintTimer <= 0.0f ){	//if really close && we can sprint again (if not too tired)
					turbo = maxTurbo * ofRandom(4, 6);
					sprintTimer = maxSprintTimer;
				}
			}

			if ( clan == mate->clan ){
				if ( dist < params->schoolFriendsDist * params->schoolFriendsDist ){
					schoolSameColor = schoolSameColor + ( mate->pos - pos );
				}
			}

			if ( clan != mate->clan ){
				if ( dist < params->schoolOthersDist * params->schoolOthersDist ){
					schoolDifferentColor = schoolDifferentColor - mate->pos + pos ;
					if (sprintTimer <= 0.0){
						turbo = maxTurbo * ofRandom(3, 5);
						sprintTimer = maxSprintTimer;
					}
				}
			}

			if ( dist < params->cohesionDist * params->cohesionDist ){
				if ( clan == mate->clan ){
					groupCohesion = groupCohesion + mate->vel;
				}
			}
		}
	}
}


void FlockMember::draw(ofMesh & pointsMesh, ofMesh &forcesMesh){

	if (!enabled)
		return;
	if (false){ //3d model orientation

		glPushMatrix();
			glTranslatef( pos.x, pos.y, pos.z );

			ofQuaternion rot;
			rot.makeRotate(vel.normalized(), ofVec3f(1,0,0) );
			ofVec3f r = rot.getEuler();
			glRotatef( -r.y, 0, 1, 0);
			glRotatef( -r.x, 0, 0, 1);
			//glRotatef( -r.z, 1, 0, 0);

			ofSetColor(c);
			//model->draw();

		glPopMatrix();
	}else{
		ofSetColor(c);
		//ofSphere(pos, 0.1);
		pointsMesh.addColor(c);
		pointsMesh.addVertex(pos);
	}
	//ofSetColor(c);

	float gain = 30;

	if (params->debugShowDirection){
		ofColor cc = ofColor::yellow;
		forcesMesh.addColor(cc);
		forcesMesh.addVertex(pos);
		cc.a = 0;
		forcesMesh.addColor(cc);
		forcesMesh.addVertex(pos + vel);
	}

	if (params->debugShowCollisions){
		ofColor cc = ofColor::red;
		forcesMesh.addColor(cc);
		forcesMesh.addVertex(pos);
		cc.a = 0;
		forcesMesh.addColor(cc);
		forcesMesh.addVertex(pos + avoidNeigbhors * gain);
	}

	if (params->debugShowCohesion){
		ofColor cc = ofColor::blue;
		forcesMesh.addColor(cc);
		forcesMesh.addVertex(pos);
		cc.a = 0;
		forcesMesh.addColor(cc);
		forcesMesh.addVertex(pos + groupCohesion * gain);
	}

	if (params->debugShowSchooling){
		ofColor cc = ofColor::magenta;
		forcesMesh.addColor(cc);
		forcesMesh.addVertex(pos);
		cc.a = 0;
		forcesMesh.addColor(cc);
		forcesMesh.addVertex(pos + schoolSameColor * gain);

		cc = ofColor::cyan;
		forcesMesh.addColor(cc);
		forcesMesh.addVertex(pos);
		cc.a = 0;
		forcesMesh.addColor(cc);
		forcesMesh.addVertex(pos + schoolDifferentColor * gain);
	}

	float ballAlpha = 128;
	if (params->debugCohesionDist){
		ofNoFill();
		ofSetColor(0,0,255,ballAlpha );
		ofSphere(pos, params->cohesionDist * 0.5f);
		ofFill();
	}

	if (params->debugCollisionDist){
		ofNoFill();
		ofSetColor(255,0,0,ballAlpha );
		ofSphere(pos, params->collisionDist * 0.5f);
		ofFill();
	}

	if (params->debugSchoolingFriendsDist){
		ofNoFill();
		ofSetColor(255,0,255,ballAlpha );
		ofSphere(pos, params->schoolFriendsDist * 0.5f);
		ofFill();
	}

	if (params->debugSchoolingOthersDist){
		ofNoFill();
		ofSetColor(0,255,255,ballAlpha );
		ofSphere(pos, params->schoolOthersDist * 0.5f);
		ofFill();
	}
//	ofSetColor(255);
//	ofDrawBitmapString(ofToString(vel), pos);
}

