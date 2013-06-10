#pragma once

#include "ofMain.h"
#include "ofxColorGradient.h"
#include "Trails.h"
#include "ofxFboBlur.h"
#include "ofxRemoteUIServer.h"

#define NUM_TRAILS 50

class testApp : public ofBaseApp{

	struct RandomGenerator{

		RandomGenerator(){
			r = ofRandom(100,300);
			t0 = ofRandom(-1.0, 1.0);
			t1 = ofRandom(-1.0, 1.0);
			t2 = ofRandom(-1.0, 1.0);
			t3 = ofRandom(-1.0, 1.0);
			t4 = ofRandom(-1.0, 1.0);
			t5 = ofRandom(-1.0, 1.0);
			off0 = ofRandom(-100, 100);
			off1 = ofRandom(-100, 100);
			off2 = ofRandom(-100, 100);
			off3 = ofRandom(-100, 100);
			off4 = ofRandom(-100, 100);
			off5 = ofRandom(-100, 100);
		}

		ofVec3f getPos(){
			ofVec3f p;
			p.x = 2 * r * sin(ofGetElapsedTimef() + off0) * ofNoise(ofGetElapsedTimef() * t1 + off1);
			p.y = r * cosf( t2 * ofGetElapsedTimef() + off2) + r * sinf( t3 * ofGetElapsedTimef() + off3) / 2;
			p.z = r * cosf( t4 * ofGetElapsedTimef() + off4) + r * sinf( t5 * ofGetElapsedTimef() + off5);
			return p;
		}

		float r;
		float t1, t2, t3, t4, t5, t0;
		float off1, off2, off3, off4, off5, off0;
	};

	public:
		void setup();
		void update();
		void draw();

	void exit();
	
		void keyPressed  (int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);

	Trails t[NUM_TRAILS];
	RandomGenerator rg[NUM_TRAILS];
	
	ofEasyCam cam;
	bool run;
	float lineW;

	ofxFboBlur gpuBlur;
	ofLight light;
};
