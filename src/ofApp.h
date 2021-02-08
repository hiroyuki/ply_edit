#pragma once

#include "ofMain.h"
#include "tinyply.h"
#include "plyEditor.h"
#include "ofxAutoReloadedShader.h"
#include "ofxGui.h"


class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
		plyEditor editor;
		ofEasyCam camera;
		ofxAutoReloadedShader shader;
		ofxPanel gui;
		ofxFloatSlider hue, saturation, brightness, scale;
		ofxButton loadBtn;
		ofxButton writeBtn;

		void loadPLY()
		{
			ofFileDialogResult r = ofSystemLoadDialog("open PLY");
			ofLog() << r.getPath();
		}

		void exportPLY()
		{
		//	editor.save(ofGetTimestampString()+".ply", hue, saturation, brightness);
			string filename = "hue_"+ofToString(float(hue)) + ".satu_" + ofToString(float(saturation)) + ".bri_" + ofToString(float(brightness)) + ".ply";
			editor.save(filename, hue, saturation, brightness);
		}

};
