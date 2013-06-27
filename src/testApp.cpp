#include "testApp.h"
#include <GLUT/GLUT.h>

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
		float s = 0.5;
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
	s.numSamples = 8;
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

	shader = new ofxAutoReloadedShader();
	shader->setMillisBetweenFileCheck(500);
	shader->load("shaders/phong");


	OFX_REMOTEUI_SERVER_SETUP(10000); 	//start server

	OFX_REMOTEUI_SERVER_SET_UPCOMING_PARAM_GROUP("SHADER");
	OFX_REMOTEUI_SERVER_SHARE_PARAM(doShader);

	vector<string> showL; showL.push_back("SHOW_ALL"); showL.push_back("SHOW_N");
	showL.push_back("SHOW_L"); showL.push_back("SHOW_E"); showL.push_back("SHOW_R");
	showL.push_back("SHOW_SPECULAR");showL.push_back("SHOW_EYE_SPECULAR");
	OFX_REMOTEUI_SERVER_SHARE_ENUM_PARAM(showInShader, SHOW_ALL, NUM_SHOWS-1, showL);

	OFX_REMOTEUI_SERVER_SET_UPCOMING_PARAM_COLOR( ofColor(255,0,255,64) ); // set a bg color for the upcoming params
	OFX_REMOTEUI_SERVER_SHARE_COLOR_PARAM(matAmbient);
	OFX_REMOTEUI_SERVER_SHARE_COLOR_PARAM(matDiffuse);
	OFX_REMOTEUI_SERVER_SHARE_COLOR_PARAM(matSpecular);

	OFX_REMOTEUI_SERVER_SET_UPCOMING_PARAM_COLOR( ofColor(0,255,0,64) ); // set a bg color for the upcoming params
	OFX_REMOTEUI_SERVER_SHARE_PARAM(specularGain,0,2);
	OFX_REMOTEUI_SERVER_SHARE_PARAM(specularClamp,0,1);
	OFX_REMOTEUI_SERVER_SHARE_PARAM(specularPow,0,1);

	OFX_REMOTEUI_SERVER_SET_UPCOMING_PARAM_COLOR( ofColor(255,255,0,64) ); // set a bg color for the upcoming params
	OFX_REMOTEUI_SERVER_SHARE_PARAM(eyeSpecularGain,0,2);
	OFX_REMOTEUI_SERVER_SHARE_PARAM(eyeSpecularClamp,0,1);
	OFX_REMOTEUI_SERVER_SHARE_PARAM(eyeSpecularPow,0,1);

	OFX_REMOTEUI_SERVER_SET_UPCOMING_PARAM_GROUP("LIGHTS");
	OFX_REMOTEUI_SERVER_SET_UPCOMING_PARAM_COLOR( ofColor(255,255,255,64) ); // set a bg color for the upcoming params
	OFX_REMOTEUI_SERVER_SHARE_COLOR_PARAM(lightAmbient);
	OFX_REMOTEUI_SERVER_SHARE_COLOR_PARAM(lightDiffuse);
	OFX_REMOTEUI_SERVER_SHARE_COLOR_PARAM(lightSpecular);
	OFX_REMOTEUI_SERVER_SET_UPCOMING_PARAM_COLOR( ofColor(255,128,0,64) ); // set a bg color for the upcoming params
	OFX_REMOTEUI_SERVER_SHARE_PARAM(animateLight);
	OFX_REMOTEUI_SERVER_SHARE_PARAM(lightSpeed,0,10);
	OFX_REMOTEUI_SERVER_SHARE_PARAM(lightDist,1,600);
	OFX_REMOTEUI_SERVER_SHARE_PARAM(drawTeapot);



	OFX_REMOTEUI_SERVER_SET_UPCOMING_PARAM_GROUP("BLUR");
	OFX_REMOTEUI_SERVER_SET_UPCOMING_PARAM_COLOR( ofColor(0,0,255,32) ); // set a bg color for the upcoming params
	OFX_REMOTEUI_SERVER_SHARE_PARAM(gpuBlur.blurPasses, 0, 4);
	OFX_REMOTEUI_SERVER_SHARE_PARAM(gpuBlur.blurOffset, 0.0, 10);
	OFX_REMOTEUI_SERVER_SHARE_PARAM(gpuBlur.blurOverlayGain, 0, 255);
	OFX_REMOTEUI_SERVER_SHARE_PARAM(gpuBlur.numBlurOverlays, 0, 7);

	OFX_REMOTEUI_SERVER_SET_UPCOMING_PARAM_GROUP("TRAILS");
	OFX_REMOTEUI_SERVER_SET_UPCOMING_PARAM_COLOR( ofColor(0,255,0,32) ); // set a bg color for the upcoming params
	OFX_REMOTEUI_SERVER_SHARE_PARAM(lineW, 0.1, 10);

	OFX_REMOTEUI_SERVER_SHARE_PARAM(drawTrails);
	OFX_REMOTEUI_SERVER_SHARE_PARAM(tp.radius, 0.01, 50);
	OFX_REMOTEUI_SERVER_SHARE_PARAM(tp.flatness, 0, 1.0);
	OFX_REMOTEUI_SERVER_SHARE_PARAM(tp.maxLength, 0, 250);
	OFX_REMOTEUI_SERVER_SHARE_PARAM(tp.skipStep, 1, 20);
	OFX_REMOTEUI_SERVER_SHARE_PARAM(tp.circleRes, 4, 20);
	OFX_REMOTEUI_SERVER_SET_UPCOMING_PARAM_COLOR( ofColor(128,0,255,32) ); // set a bg color for the upcoming params
	OFX_REMOTEUI_SERVER_SHARE_PARAM(tp.headLen, 0, .9);
	OFX_REMOTEUI_SERVER_SHARE_PARAM(tp.tailLen, 0, .9);
	OFX_REMOTEUI_SERVER_SET_UPCOMING_PARAM_COLOR( ofColor(255,128,0,32) ); // set a bg color for the upcoming params
	OFX_REMOTEUI_SERVER_SHARE_PARAM(tp.ondulationAmp, 0, 90);
	OFX_REMOTEUI_SERVER_SHARE_PARAM(tp.ondulationFreq, 0, 40);
	OFX_REMOTEUI_SERVER_SHARE_PARAM(tp.ondulationPeriod, 3, 20);
	OFX_REMOTEUI_SERVER_SET_UPCOMING_PARAM_COLOR( ofColor(0,0,255,32) ); // set a bg color for the upcoming params
	//MACRO acces wont work for enum types, we need to cast, so we do it manually
	vector<string> curveNames;
	for(int i = 0; i<NUM_ANIM_CURVES; i++){
		curveNames.push_back(ofxAnimatable::getCurveName((AnimCurve)i));
	}
	ofxRemoteUIServer::instance()->shareParam( "tp.headTailcurve", (int*)&tp.headTailcurve, 0, NUM_ANIM_CURVES-1, curveNames);

	vector<string> modesNames;
	modesNames.push_back("OF_PRIMITIVE_TRIANGLES"); modesNames.push_back("OF_PRIMITIVE_TRIANGLE_STRIP");
	modesNames.push_back("OF_PRIMITIVE_TRIANGLE_FAN"); modesNames.push_back("OF_PRIMITIVE_LINES");
	modesNames.push_back("OF_PRIMITIVE_LINE_STRIP"); modesNames.push_back("OF_PRIMITIVE_LINE_LOOP");
	modesNames.push_back("OF_PRIMITIVE_LINE_POINTS");
	ofxRemoteUIServer::instance()->shareParam( "tp.primitiveMode", (int*)&tp.primitiveMode, OF_PRIMITIVE_TRIANGLES, OF_PRIMITIVE_POINTS, modesNames);
	OFX_REMOTEUI_SERVER_SHARE_PARAM(tp.drawNormals);


	OFX_REMOTEUI_SERVER_SET_UPCOMING_PARAM_GROUP("FLOCK");
	OFX_REMOTEUI_SERVER_SET_UPCOMING_PARAM_COLOR( ofColor(255,255,255,32) ); // set a bg color for the upcoming params
	OFX_REMOTEUI_SERVER_SHARE_PARAM(fp.ignoreClans);
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
	OFX_REMOTEUI_SERVER_SHARE_PARAM(fp.collisionAvoidF, 0, 1);
	OFX_REMOTEUI_SERVER_SHARE_PARAM(fp.collisionDist, 0, distRange);
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
	OFX_REMOTEUI_SERVER_SHARE_PARAM(fp.maxSpeedMagnitude, 0, 300);
	OFX_REMOTEUI_SERVER_SHARE_PARAM(fp.accelerationF, 0, 150);
	OFX_REMOTEUI_SERVER_SHARE_PARAM(fp.mediumFriction, 0.5, 1);
	OFX_REMOTEUI_SERVER_SET_UPCOMING_PARAM_COLOR( ofColor(0,0,177,32) ); // set a bg color for the upcoming params
	OFX_REMOTEUI_SERVER_SHARE_PARAM(fp.rethinkTime, 0, 0.4);

	OFX_REMOTEUI_SERVER_SET_UPCOMING_PARAM_COLOR( ofColor(255,255,0,32) ); // set a bg color for the upcoming params
	OFX_REMOTEUI_SERVER_SHARE_PARAM(fp.debugShowDirection);

	flock->setup();

	OFX_REMOTEUI_SERVER_LOAD_FROM_XML();


	light.enable();
	light.setPointLight();
	TIME_SAMPLE_SET_FRAMERATE(60);
	//ofEnableSmoothing();
	//cam.setDrag(0.98);

}

//--------------------------------------------------------------
void testApp::update(){

	OFX_REMOTEUI_SERVER_UPDATE(0.01666);
	//t.update()

	mMatMainMaterial.setAmbientColor( matAmbient);
	mMatMainMaterial.setDiffuseColor(matDiffuse);
	mMatMainMaterial.setSpecularColor(matSpecular);
	mMatMainMaterial.setShininess(25.0f);

	light.setAmbientColor(lightAmbient);
	light.setDiffuseColor(lightDiffuse);
	light.setSpecularColor(lightSpecular);


	TIME_SAMPLE_START("updateFlock");
	if (run){
		flock->update( 0.016666666f);
		for(int i = 0 ; i < NUM_FISH_PER_FLOCK; i++){
			t[i].update( flock->getMember(i)->pos);
		}			
	}
	TIME_SAMPLE_STOP("updateFlock");

	TIME_SAMPLE_START("generateMesh");
	for(int i = 0 ; i < NUM_FISH_PER_FLOCK; i++){
		t[i].generateMesh();
	}
	TIME_SAMPLE_STOP("generateMesh");

	if(animateLight || ofGetFrameNum() < 2){
		float r = lightDist;
		lightPos = ofVec3f(r * sin(lightSpeed * ofGetElapsedTimef()),
						   250,
						   r * cos(lightSpeed * ofGetElapsedTimef()) );
	}
	light.setGlobalPosition(lightPos);

}

//--------------------------------------------------------------
void testApp::draw(){

	ofSetLineWidth(lineW);
	glPointSize(lineW);

	//glEnable(GL_NORMALIZE);

	gpuBlur.beginDrawScene();
		ofClear(0, 0, 0, 0);

		cam.begin();

			light.enable();

			glEnable(GL_DEPTH_TEST);
			//glShadeModel(GL_FLAT);

			ofSetColor(255);
			light.draw();

			ofEnableLighting();
			ofSetSphereResolution(4);

			flock->draw();

			glDisable(GL_COLOR_MATERIAL);
			TIME_SAMPLE_START("drawWorms");
			ofEnableBlendMode(OF_BLENDMODE_ADD);
			ofEnableBlendMode(OF_BLENDMODE_ALPHA);

			mMatMainMaterial.begin();
			if(doShader){
				shader->begin();
				shader->setUniform1f("shaderVal1", 1);
				shader->setUniform1f("shaderVal2", 1);
				shader->setUniform1f("shaderVal3", 1);
				shader->setUniform1f("time", ofGetElapsedTimef() );
				shader->setUniform1f("specularGain", specularGain);
				shader->setUniform1f("specularClamp", specularClamp);
				shader->setUniform1f("specularPow", specularPow);
				shader->setUniform1f("eyeSpecularGain", eyeSpecularGain);
				shader->setUniform1f("eyeSpecularClamp", eyeSpecularClamp);
				shader->setUniform1f("eyeSpecularPow", eyeSpecularPow);
				shader->setUniform1f("showInShader", showInShader);
			}

			if(drawTeapot) glutSolidTeapot(80);

			if(drawTrails){
				for(int i = 0; i < NUM_FISH_PER_FLOCK; i++){
					t[i].draw();
				}
			}
			if(doShader){
				shader->end();
			}
			mMatMainMaterial.end();

			ofDisableLighting();
			TIME_SAMPLE_STOP("drawWorms");

			ofDrawAxis(10);
			ofSetColor(255);
			
			glDisable(GL_DEPTH_TEST);
		
		cam.end();

	gpuBlur.endDrawScene();

	ofSetColor(255);
	ofEnableBlendMode(OF_BLENDMODE_ALPHA);
	gpuBlur.performBlur();

	//draw clean scene
	ofSetColor(255);
	ofBackgroundGradient(ofColor::fromHsb(160, 255, 60), ofColor::fromHsb(0, 0, 0));

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

	if (key=='r'){
		flock->resetPositions();
		for(int i = 0; i < NUM_FISH_PER_FLOCK; i++){
			t[i].clear();
		}
		
	}else{
		run = !run;
	}
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
	OFX_REMOTEUI_SERVER_CLOSE();
	OFX_REMOTEUI_SERVER_SAVE_TO_XML();
}
