#include "ofMain.h"
#include "testApp.h"
#include "ofAppGlutWindow.h"

//========================================================================
int main( ){

    ofAppGlutWindow window;
	window.setGlutDisplayString("rgba double samples>=8 depth");

	ofSetupOpenGL(&window, 2048, 1152, OF_FULLSCREEN);
	//ofSetupOpenGL(&window, 2048, 1152, OF_WINDOW);


	// this kicks off the running of my app
	// can be OF_WINDOW or OF_FULLSCREEN
	// pass in width and height too:
	ofRunApp( new testApp());

}
