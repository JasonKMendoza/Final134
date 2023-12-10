#pragma once

#include "ofMain.h"
#include "ofxGui.h"
#include  "ofxAssimpModelLoader.h"
#include "Octree.h"
#include <cmath>

class physicsModel {
public:
	glm::vec3 velocity = glm::vec3(0, 0, 0);
	glm::vec3 acceleration = glm::vec3(0, 0, 0);
	glm::vec3 force = glm::vec3(0, 0, 0);
	float angularVel = 0;
	float angularAccel = 0;
	float angularForce = 0;
	float mass = 1.0;
	float damping = .80;
	float angle = 0;
	ofxAssimpModelLoader model;
	glm::vec3 heading;

	void integrate() {
		float dt = 1.0 / ofGetFrameRate();
		glm::vec3 currPos = model.getPosition();
		currPos += (velocity * dt);
		model.setPosition(currPos.x, currPos.y, currPos.z);
		acceleration += (force * 1.0 / mass);
		velocity += acceleration * dt;
		velocity *= damping;

		//angular stuff
		angle += (angularVel * dt);
		angle = fmodf(angle, 360);
		model.setRotation(0,angle,0,1,0);
		angularAccel += (angularForce * 1.0 / mass);
		angularVel += angularAccel * dt;
		angularVel *= damping;


		// reset all forces
		angularForce = 0;
		force = glm::vec3(0, 0, 0);
	}
};

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
		void dragEvent2(ofDragInfo dragInfo);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
		void drawAxis(ofVec3f);
		void initLightingAndMaterials();
		void savePicture();
		void toggleWireframeMode();
		void togglePointsDisplay();
		void toggleSelectTerrain();
		void setCameraTarget();
		bool mouseIntersectPlane(ofVec3f planePoint, ofVec3f planeNorm, ofVec3f &point);
		bool raySelectWithOctree(ofVec3f &pointRet);
		glm::vec3 ofApp::getMousePointOnPlane(glm::vec3 p , glm::vec3 n);

		ofEasyCam cam;
		ofxAssimpModelLoader skybox;
		ofxAssimpModelLoader mars;
		physicsModel lander;
		Box boundingBox, landerBounds;
		Box testBox;
		vector<Box> colBoxList;
		bool bLanderSelected = false;
		Octree octree;
		TreeNode selectedNode;
		glm::vec3 mouseDownPos, mouseLastPos;
		bool bInDrag = false;
		map<int, bool> keymap;
		int fuel = 120000;
		bool explode = false;
		float altitude = 0;

		// Lighting
		vector<ofLight> lights;
		ofLight light1;
		ofLight light2;
		ofLight light3;
		ofLight light4;
		ofLight light5;
		ofLight light6;
		ofLight light7;

		ofxIntSlider numLevels;
		ofxPanel gui;

		bool bAltKeyDown;
		bool bCtrlKeyDown;
		bool bWireframe;
		bool bDisplayPoints;
		bool bPointSelected;
		bool bHide;
		bool pointSelected = false;
		bool bDisplayLeafNodes = false;
		bool bDisplayOctree = false;
		bool bDisplayBBoxes = false;
		bool start = false;
		
		//bool true;
		bool bTerrainSelected;
	
		ofVec3f selectedPoint;
		ofVec3f intersectPoint;

		vector<Box> bboxList;

		const float selectionRange = 4.0;

		float startTime;
		float finalTime;
};
