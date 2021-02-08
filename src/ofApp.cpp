#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){

	editor.loadPLY(ofToDataPath("Ship0.018_Surround0.018-0.07.ply"));
//	editor.loadPLY(ofToDataPath("out.ply"));
	shader.load("shader/shader");
	ofSetColor(255);
	
	loadBtn.addListener(this, &ofApp::loadPLY);
	writeBtn.addListener(this, &ofApp::exportPLY);
	gui.setup();
	gui.add(scale.setup("scale", 50, 20, 80));

	gui.add(hue.setup("hue", 1, 0, 3));
	gui.add(saturation.setup("satulation", 1, 0, 3));
	gui.add(brightness.setup("brightness", 1, 0, 3));
	gui.add(loadBtn.setup("load"));
	gui.add(writeBtn.setup("save"));

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
	shader.begin();
	shader.setUniformMatrix4f("mvp", camera.getModelViewProjectionMatrix());
	shader.setUniform3f("hsv", hue, saturation, brightness);
	shader.setUniform1f("scale", scale);
	editor.debugDraw();
	shader.end();
	camera.end();
	ofPopStyle();

	gui.draw();
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
