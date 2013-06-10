#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){

	run = true;
	ofSetVerticalSync(true);
	ofEnableAlphaBlending();
	//ofEnableBlendMode(OF_BLENDMODE_ADD);
	ofBackground(00);
	ofxColorGradient g;
	g.addColor(ofColor::red);
	g.addColor(ofColor::green);
	g.addColor(ofColor::blue);
	g.addColor(ofColor::yellow);
	g.addColor(ofColor::orange);
//	g.addColor(ofColor(255,64));
//	g.addColor(ofColor(255,0,0,64));
	for(int i = 0; i < NUM_TRAILS; i++){
		t[i].setup(g);
	}


	ofFbo::Settings s;
	s.width = 2048;
	s.height = 1024;
	s.internalformat = GL_RGBA;
	//	s.textureTarget = GL_TEXTURE_RECTANGLE_ARB;
	s.maxFilter = GL_LINEAR; GL_NEAREST;
	s.numSamples = 4;
	s.numColorbuffers = 3;
	s.useDepth = true;
	s.useStencil = false;

	gpuBlur.setup(s);

	OFX_REMOTEUI_SERVER_SETUP(10000); 	//start server

	OFX_REMOTEUI_SERVER_SET_UPCOMING_PARAM_COLOR( ofColor(0,0,255,32) ); // set a bg color for the upcoming params
	OFX_REMOTEUI_SERVER_SHARE_PARAM(gpuBlur.blurPasses, 0, 4);
	OFX_REMOTEUI_SERVER_SHARE_PARAM(gpuBlur.blurOffset, 0.0, 10);
	OFX_REMOTEUI_SERVER_SHARE_PARAM(gpuBlur.blurOverlayGain, 0, 255);
	OFX_REMOTEUI_SERVER_SHARE_PARAM(gpuBlur.numBlurOverlays, 0, 7);

	OFX_REMOTEUI_SERVER_SET_UPCOMING_PARAM_COLOR( ofColor(0,255,0,32) ); // set a bg color for the upcoming params
	OFX_REMOTEUI_SERVER_SHARE_PARAM(lineW, 0, 10);

	OFX_REMOTEUI_SERVER_LOAD_FROM_XML();

	light.setPosition(0, 200, 0);
    light.setDiffuseColor( ofColor(255.f, 255.f, 255.f));

    // specular color, the highlight/shininess color //
	light.setSpecularColor( ofColor(255.f, 255.f, 255.f));
	light.setPointLight();
	light.enable();

}

//--------------------------------------------------------------
void testApp::update(){

	OFX_REMOTEUI_SERVER_UPDATE(0.01666);
	//t.update()
}

//--------------------------------------------------------------
void testApp::draw(){

	ofSetLineWidth(lineW);

	ofEnableLighting();
	glEnable(GL_NORMALIZE);

	gpuBlur.beginDrawScene();
	ofClear(0, 0, 0, 0);
	glEnable(GL_DEPTH_TEST);
	cam.begin();

	light.draw();

	ofEnableBlendMode(OF_BLENDMODE_ALPHA);
	if (run){
		//if (ofGetFrameNum()%3 == 1)
		for(int i = 0; i < NUM_TRAILS; i++){
			t[i].update( rg[i].getPos() );
		}
	}

	for(int i = 0; i < NUM_TRAILS; i++){
		t[i].draw();
	}

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
