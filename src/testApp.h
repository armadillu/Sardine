#pragma once

#include "ofMain.h"
#include "ofxColorGradient.h"
#include "Trails.h"
#include "ofxFboBlur.h"
#include "ofxRemoteUIServer.h"
#include "ofxTimeMeasurements.h"
#include "Flock.h"
#include "FlockParams.h"

class testApp : public ofBaseApp{



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

	Trails t[NUM_FISH_PER_FLOCK];
	TrailParams tp;
	
	ofEasyCam cam;
	bool run;
	float lineW;

	Flock * flock;
	FlockParams fp;


	bool drawTrails;

	ofxFboBlur gpuBlur;
	ofLight light;
};
