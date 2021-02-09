#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){

	ship.setup("Ship", ofToDataPath("Ship.ply"), 20, 20);
	surround.setup("Surround", ofToDataPath("Surround.ply"), 20, 150);
//	editor.loadPLY(ofToDataPath("out.ply"));
	ofSetColor(255);
	ofSetWindowShape(1920, 1080);

}

//--------------------------------------------------------------
void ofApp::update(){

}

//--------------------------------------------------------------
void ofApp::draw(){
	ofBackground(ofColor::black);
	ofPushStyle();
	camera.begin();
	ship.debugDraw(camera.getModelViewProjectionMatrix());
	surround.debugDraw(camera.getModelViewProjectionMatrix());
	camera.end();
	ofPopStyle();
	ship.drawGui();
	surround.drawGui();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
