#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){

	run = true;
	ofSetVerticalSync(true);
	ofEnableAlphaBlending();
	//ofEnableBlendMode(OF_BLENDMODE_ADD);
	ofBackground(00);

	flock = new Flock(&fp);

	//inital Trail settings
	tp.radius = 22;
	tp.circleRes = 8;
	tp.skipStep = 5;
	tp.headTailcurve = QUADRATIC_EASE_OUT;
	tp.maxLength = 400;

	int lineA = 255;
	for(int i = 0; i < NUM_FISH_PER_FLOCK; i++){
		ofxColorGradient g;
//		for(int j = 0; j < 3; j++){
//			g.addColor( ofColor(	ofRandom(255),
//									ofRandom(255),
//									ofRandom(255),
//									lineA)
//					   );
//		}
		ofColor c = flock->getMember(i)->c;
		float s = 0.0;
		c.r = c.r * s;
		c.g = c.g * s;
		c.b = c.b * s;
		g.addColor(c);
		g.addColor(flock->getMember(i)->c);
		t[i].setup(g, &tp);
	}
	glEnable(GL_LINE_SMOOTH);


	ofFbo::Settings s;
	s.width = ofGetWidth();
	s.height = ofGetHeight();
	s.internalformat = GL_RGBA;
	//	s.textureTarget = GL_TEXTURE_RECTANGLE_ARB;
	s.maxFilter = GL_LINEAR; GL_NEAREST;
	s.numSamples = 4;
	s.numColorbuffers = 3;
	s.useDepth = true;
	s.useStencil = false;

	gpuBlur.setup(s);

	//initial params
	fp.rethinkTime = 0.03f;

	//distances
	fp.schoolFriendsDist = 5.8;
	fp.schoolOthersDist = 2.9f;
	fp.collisionDist = 0.93f;
	fp.cohesionDist = 4.04f;

	//Forces
	fp.schoolFriends = 0.44f;
	fp.schoolOthersF = 0.14f;
	fp.collisionAvoidF = 0.35f;
	fp.cohesionF = 0.35f;
	fp.centerismF = 0.001f;

	fp.maxSpeedMagnitude = 2.1;
	fp.accelerationF = 0.15f;

	fp.debugShowCollisions = false;
	fp.debugShowCohesion = false;
	fp.debugShowSchooling = false;
	fp.debugShowDirection = false;

	float distRange = 200;

	OFX_REMOTEUI_SERVER_SETUP(10000); 	//start server

	OFX_REMOTEUI_SERVER_SET_UPCOMING_PARAM_COLOR( ofColor(0,0,255,32) ); // set a bg color for the upcoming params
	OFX_REMOTEUI_SERVER_SHARE_PARAM(gpuBlur.blurPasses, 0, 4);
	OFX_REMOTEUI_SERVER_SHARE_PARAM(gpuBlur.blurOffset, 0.0, 10);
	OFX_REMOTEUI_SERVER_SHARE_PARAM(gpuBlur.blurOverlayGain, 0, 255);
	OFX_REMOTEUI_SERVER_SHARE_PARAM(gpuBlur.numBlurOverlays, 0, 7);

	OFX_REMOTEUI_SERVER_SET_UPCOMING_PARAM_COLOR( ofColor(0,255,0,32) ); // set a bg color for the upcoming params
	OFX_REMOTEUI_SERVER_SHARE_PARAM(lineW, 0.1, 10);

	OFX_REMOTEUI_SERVER_SHARE_PARAM(drawTrails);
	OFX_REMOTEUI_SERVER_SHARE_PARAM(tp.radius, 0.01, 20);
	OFX_REMOTEUI_SERVER_SHARE_PARAM(tp.maxLength, 0, 100);
	OFX_REMOTEUI_SERVER_SHARE_PARAM(tp.skipStep, 1, 20);
	OFX_REMOTEUI_SERVER_SHARE_PARAM(tp.circleRes, 4, 10);
	OFX_REMOTEUI_SERVER_SHARE_PARAM(tp.headLen, 0, 1);
	OFX_REMOTEUI_SERVER_SHARE_PARAM(tp.tailLen, 0, 1);

	OFX_REMOTEUI_SERVER_SET_UPCOMING_PARAM_COLOR( ofColor(255,0,255,32) ); // set a bg color for the upcoming params
	OFX_REMOTEUI_SERVER_SHARE_PARAM(fp.schoolFriends, 0, 1);
	OFX_REMOTEUI_SERVER_SHARE_PARAM(fp.schoolFriendsDist, 0, distRange);
	OFX_REMOTEUI_SERVER_SHARE_PARAM(fp.debugShowSchooling);
	OFX_REMOTEUI_SERVER_SHARE_PARAM(fp.debugSchoolingFriendsDist);

	OFX_REMOTEUI_SERVER_SET_UPCOMING_PARAM_COLOR( ofColor(0,255,255,32) ); // set a bg color for the upcoming params
	OFX_REMOTEUI_SERVER_SHARE_PARAM(fp.schoolOthersF, 0, 1);
	OFX_REMOTEUI_SERVER_SHARE_PARAM(fp.schoolOthersDist, 0, distRange);
	OFX_REMOTEUI_SERVER_SHARE_PARAM(fp.debugSchoolingOthersDist);

	OFX_REMOTEUI_SERVER_SET_UPCOMING_PARAM_COLOR( ofColor(255,0,0,32) ); // set a bg color for the upcoming params
	OFX_REMOTEUI_SERVER_SHARE_PARAM(fp.collisionDist, 0, distRange);
	OFX_REMOTEUI_SERVER_SHARE_PARAM(fp.collisionAvoidF, 0, 1);
	OFX_REMOTEUI_SERVER_SHARE_PARAM(fp.debugShowCollisions);
	OFX_REMOTEUI_SERVER_SHARE_PARAM(fp.debugCollisionDist);

	OFX_REMOTEUI_SERVER_SET_UPCOMING_PARAM_COLOR( ofColor(0,0,255,32) ); // set a bg color for the upcoming params
	OFX_REMOTEUI_SERVER_SHARE_PARAM(fp.cohesionF, 0, 1);
	OFX_REMOTEUI_SERVER_SHARE_PARAM(fp.cohesionDist, 0, distRange);
	OFX_REMOTEUI_SERVER_SHARE_PARAM(fp.debugShowCohesion);
	OFX_REMOTEUI_SERVER_SHARE_PARAM(fp.debugCohesionDist);

	OFX_REMOTEUI_SERVER_SET_UPCOMING_PARAM_COLOR( ofColor(255,32) ); // set a bg color for the upcoming params
	OFX_REMOTEUI_SERVER_SHARE_PARAM(fp.centerismF, 0, 1);
	OFX_REMOTEUI_SERVER_SHARE_PARAM(fp.centerRadius, 50, 300);

	OFX_REMOTEUI_SERVER_SET_UPCOMING_PARAM_COLOR( ofColor(250,0,177,32) ); // set a bg color for the upcoming params
	OFX_REMOTEUI_SERVER_SHARE_PARAM(fp.maxSpeedMagnitude, 0, 100);
	OFX_REMOTEUI_SERVER_SHARE_PARAM(fp.accelerationF, 0, 50);
	OFX_REMOTEUI_SERVER_SHARE_PARAM(fp.mediumFriction, 0.5, 1);
	OFX_REMOTEUI_SERVER_SET_UPCOMING_PARAM_COLOR( ofColor(0,0,177,32) ); // set a bg color for the upcoming params
	OFX_REMOTEUI_SERVER_SHARE_PARAM(fp.rethinkTime, 0, 0.4);

	OFX_REMOTEUI_SERVER_SET_UPCOMING_PARAM_COLOR( ofColor(255,255,0,32) ); // set a bg color for the upcoming params
	OFX_REMOTEUI_SERVER_SHARE_PARAM(fp.debugShowDirection);


	OFX_REMOTEUI_SERVER_LOAD_FROM_XML();


	light.setPosition(0, 200, 0);
    light.setDiffuseColor( ofColor(255.f, 255.f, 255.f));

    // specular color, the highlight/shininess color //
	light.setSpecularColor( ofColor(255.f, 255.f, 255.f));
	light.setPointLight();
	light.enable();
	TIME_SAMPLE_SET_FRAMERATE(60);
	ofEnableSmoothing();

}

//--------------------------------------------------------------
void testApp::update(){

	OFX_REMOTEUI_SERVER_UPDATE(0.01666);
	//t.update()
	TIME_SAMPLE_START("update");

		TIME_SAMPLE_START("updateFlock");
		flock->update( 0.016666666f);
		for(int i = 0 ; i < NUM_FISH_PER_FLOCK; i++){
			t[i].update( flock->getMember(i)->pos);
		}
		TIME_SAMPLE_STOP("updateFlock");

	TIME_SAMPLE_STOP("update");
}

//--------------------------------------------------------------
void testApp::draw(){

	ofSetLineWidth(1);
	glPointSize(lineW);
	//glPolygonMode(GL_FRONT, GL_FILL);
	//glEnable(GL_CULL_FACE);
	//glCullFace(GL_BACK);

	ofEnableLighting();
	//glEnable(GL_NORMALIZE);

	gpuBlur.beginDrawScene();
		ofClear(0, 0, 0, 0);
		cam.begin();

			glEnable(GL_DEPTH_TEST);
			glDepthFunc(GL_LEQUAL);
			//glShadeModel(GL_FLAT);

			//light.draw();

	ofNoFill();
	ofSetSphereResolution(4);
			flock->draw();

			TIME_SAMPLE_START("drawWorms");
			ofEnableBlendMode(OF_BLENDMODE_ADD);
			ofEnableBlendMode(OF_BLENDMODE_ALPHA);
			if(drawTrails){
				for(int i = 0; i < NUM_FISH_PER_FLOCK; i++){
					t[i].draw();
				}
			}
			TIME_SAMPLE_STOP("drawWorms");

			ofDrawAxis(10);
			ofSetColor(128);
			
			glDisable(GL_DEPTH_TEST);
		
		cam.end();

	gpuBlur.endDrawScene();

	ofSetColor(255);
	ofEnableBlendMode(OF_BLENDMODE_ALPHA);
	gpuBlur.performBlur();

	//draw clean scene
	ofSetColor(255);
	ofEnableBlendMode(OF_BLENDMODE_ALPHA);
	gpuBlur.drawSceneFBO();
	ofEnableBlendMode(OF_BLENDMODE_ADD);

	//overlay the blur
	gpuBlur.drawBlurFbo();

	ofSetColor(255);
	TIME_SAMPLE_DRAW_TOP_LEFT();

}

//--------------------------------------------------------------
void testApp::keyPressed(int key){

	run = !run;
}

//--------------------------------------------------------------
void testApp::keyReleased(int key){

}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo){ 

}

void testApp::exit(){
	OFX_REMOTEUI_SERVER_SAVE_TO_XML();
}
