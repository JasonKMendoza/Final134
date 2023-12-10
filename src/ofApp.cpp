
//--------------------------------------------------------------
//
//  Kevin M. Smith
//
//  Octree Test - startup scene
// 
//
//  Student Name:   < Your Name goes Here >
//  Date: <date of last version>


#include "ofApp.h"
#include "Util.h"
#include <glm/gtx/intersect.hpp>


//--------------------------------------------------------------
// setup scene, lighting, state and load geometry
//
void ofApp::setup(){
	bWireframe = false;
	bDisplayPoints = false;
	bAltKeyDown = false;
	bCtrlKeyDown = false;
	bTerrainSelected = true;
//	ofSetWindowShape(1024, 768);

	theCam = &cam;

	mountedCam.setNearClip(.1);

	trackingCam.setNearClip(.1);
	trackingCam.setPosition(0, 40, 0);

	//Loading Font
	verdana20.load("verdana.ttf", 20, true, true);
	verdana20.setLineHeight(24.0f);
	verdana20.setLetterSpacing(1.035);

	cam.setDistance(10);
	cam.setNearClip(.1);
	cam.setFov(65.5);   // approx equivalent to 28mm in 35mm format
	ofSetVerticalSync(true);
	cam.disableMouseInput();
	ofEnableSmoothing();
	ofEnableDepthTest();

	// setup rudimentary lighting 
	//
	initLightingAndMaterials();

	if (lander.model.loadModel("geo/rocket.obj")) {
		lander.model.setScaleNormalization(false);
		lander.model.setPosition(9, 9, 9);
		cam.setPosition(10, 10, 10);
		//cout << "number of meshes: " << lander.model.getNumMeshes() << endl;
		bboxList.clear();
		for (int i = 0; i < lander.model.getMeshCount(); i++) {
			bboxList.push_back(Octree::meshBounds(lander.model.getMesh(i)));
		}

		glm::vec3 min = lander.model.getSceneMin();
		glm::vec3 max = lander.model.getSceneMax();
		float offset = (max.y - min.y) / 2.0;
		landerBounds = Box(Vector3(min.x, min.y, min.z), Vector3(max.x, max.y, max.z));
	}
	else {
		cout << "No go\n";
	}

	//mars.loadModel("geo/mars-low-5x-v2.obj");
	//mars.loadModel("geo/moon-houdini.obj");
	mars.loadModel("geo/landscape.obj");
	lava.loadModel("geo/lava.obj");
	skybox.loadModel("geo/skybox.obj");

	
	mars.setScaleNormalization(false);
	lava.setScaleNormalization(false);
	skybox.setScaleNormalization(false);


	// Lighting setup
	light1.setup();
	light1.setSpotlight();
	light1.rotate(-90, ofVec3f(1, 0, 0));
	light1.setPosition(57.6471, 17.7934 + 20, -38.8235);
	light1.setAmbientColor(ofColor::cyan);
	light1.setDiffuseColor(ofColor::cyan);
	light1.setSpecularColor(ofColor::cyan);
	light1.setAttenuation(4, 0.001, 0.001);
	light1.enable();

	light2.setup();
	light2.setSpotlight();
	light2.rotate(-90, ofVec3f(1, 0, 0));
	light2.setPosition(-74.7059, 17.9984 + 20, -108.235);
	light2.setAmbientColor(ofColor::cyan);
	light2.setDiffuseColor(ofColor::cyan);
	light2.setSpecularColor(ofColor::cyan);
	light2.setAttenuation(4, 0.001, 0.001);
	light2.enable();

	light3.setup();
	light3.setSpotlight();
	light3.rotate(-90, ofVec3f(1, 0, 0));
	light3.setPosition(26.4706, 18.111 + 20, 99.4118);
	light3.setAmbientColor(ofColor::cyan);
	light3.setDiffuseColor(ofColor::cyan);
	light3.setSpecularColor(ofColor::cyan);
	light3.setAttenuation(4, 0.001, 0.001);
	light3.enable();

	light4.setup();
	light4.setAreaLight(20, 300);
	light4.rotate(-90, ofVec3f(1, 0, 0));
	light4.setPosition(0, 0, 0);
	light4.setAmbientColor(ofColor::mediumPurple);
	light4.setDiffuseColor(ofColor::mediumPurple);
	light4.setSpecularColor(ofColor::mediumPurple);
	light4.enable();

	lights.push_back(light1);
	lights.push_back(light2);
	lights.push_back(light3);
	lights.push_back(light4);

	// create sliders for testing
	//
	gui.setup();
	gui.add(numLevels.setup("Number of Octree Levels", 1, 1, 10));

	gui.add(tVelocity.setup("T Velocity", ofVec3f(0, -20, 0), ofVec3f(0, 0, 0), ofVec3f(-100, -100, -100)));
	gui.add(tLifespan.setup("T Lifespan", 1.0, .1, 10.0));
	gui.add(tRate.setup("T Rate", 12.0, .5, 60.0));
	gui.add(tDamping.setup("T Damping", .99, .1, 1.0));
	gui.add(tGravity.setup("T Gravity", 10, 1, 20));
	gui.add(tRadius.setup("T Radius", .5, .01, .3));
	gui.add(tTurbulenceMin.setup("T TurbMin", ofVec3f(-2, -1, -3), ofVec3f(0, 0, 0), ofVec3f(-20, -20, -20)));
	gui.add(tTurbulenceMax.setup("T TurbMax", ofVec3f(1, 2, 5), ofVec3f(0, 0, 0), ofVec3f(20, 20, 20)));

	gui.add(eVelocity.setup("E Velocity", ofVec3f(0, 20, 0), ofVec3f(0, 10, 0), ofVec3f(100, 100, 100)));
	gui.add(eLifespan.setup("E Lifespan", 2.0, .1, 10.0));
	gui.add(eRate.setup("E Rate", 10.0, .5, 60.0));
	gui.add(eDamping.setup("E Damping", .99, .1, 1.0));
	gui.add(eGravity.setup("E Gravity", 50, -100, 100));
	gui.add(eRadius.setup("E Radius", .4, .01, .3));
	gui.add(eTurbulenceMin.setup("E Turbulence Min", ofVec3f(-2, -1, -3), ofVec3f(-20, -20, -20), ofVec3f(20, 20, 20)));
	gui.add(eTurbulenceMax.setup("E Turbulence Max", ofVec3f(1, 2, 5), ofVec3f(-20, -20, -20), ofVec3f(20, 20, 20)));
	gui.add(radialForceVal.setup("Radial Force", 500, 100, 5000));
	gui.add(bRadialExplosion.setup("Toggle Radial Explosion", true));
	//gui.add(ringHeight.setup("Ring Height", .01, 0.0, 1.0));

	bHide = true;

	//  Create Octree for testing.
	//
	startTime = ofGetElapsedTimeMillis();
	octree.create(mars.getMesh(0), 11);
	finalTime = ofGetElapsedTimeMillis() - startTime;
	//cout << finalTime << " ms\n";
	
	//cout << "Number of Verts: " << mars.getMesh(0).getNumVertices() << endl;

	//testBox = Box(Vector3(3, 3, 0), Vector3(5, 5, 2));
	
	
	// setup thrustEmitter
	//
	thrustEmitter.init();
	thrustEmitter.start();
	thrustEmitter.sys->addForce(new GravityForce(ofVec3f(0, -tGravity, 0))); // gravity
	thrustEmitter.sys->addForce(new TurbulenceForce(ofVec3f(-2, -1, -3), ofVec3f(1, 2, 5))); // turbulence

	// setup explosionEmitter
	turbForce = new TurbulenceForce(static_cast<ofVec3f>(eTurbulenceMin), static_cast<ofVec3f>(eTurbulenceMax));
	gravityForce = new GravityForce(ofVec3f(0, -eGravity, 0));
	radialForce = new ImpulseRadialForce(radialForceVal);
	explosionEmitter.sys->addForce(turbForce);
	explosionEmitter.sys->addForce(gravityForce);
	explosionEmitter.sys->addForce(radialForce);

	explosionEmitter.setVelocity(ofVec3f(0, 0, 0));
	explosionEmitter.setOneShot(true);
	explosionEmitter.setEmitterType(RadialEmitter);
	explosionEmitter.setGroupSize(1000);
}
 
//--------------------------------------------------------------
// incrementally update scene (animation)
//
void ofApp::update() {
	//lander.force += glm::vec3(0,.1,0);
	//lander.angularForce += 1;
	//lander.angle = 0;
	ofMatrix4x4 rotationMatrix = lander.model.getModelMatrix();
	ofVec3f upVector = rotationMatrix.getRowAsVec3f(1);
	upVector.normalize();
	lander.heading = upVector;
	//lander.heading = glm::vec3(sin(glm::radians(lander.angle)), 0, -cos(glm::radians(lander.angle)));
	//lander.force += lander.heading * .1;
	

	glm::vec3 temp = lander.model.getPosition();
	//if (theCam == &cam) cam.setPosition(temp.x + 10, temp.y + 10, temp.z + 10);

	Vector3 temp2 = Vector3(temp.x, temp.y, temp.z);
	Ray fromShip = Ray(temp2, Vector3(0, -1, 0));
	octree.intersect(fromShip, octree.root, altitude);
	fuelString = "Fuel Remaining: " + to_string(fuel);
	heightString = "Current Altitude " + to_string(altitude);

	ofVec3f min = lander.model.getSceneMin() + lander.model.getPosition();
	ofVec3f max = lander.model.getSceneMax() + lander.model.getPosition();

	Box bounds = Box(Vector3(min.x, min.y, min.z), Vector3(max.x, max.y, max.z));

	colBoxList.clear();
	octree.intersect(bounds, octree.root, colBoxList);

	if (colBoxList.size() > 1) {
		//cout << "touch\n";
		//lander.force += glm::vec3(0, .1, 0);
	}
	
	// thrust particle emitter
	thrustEmitter.setLifespan(tLifespan);
	thrustEmitter.setVelocity(static_cast<ofVec3f>(tVelocity));
	thrustEmitter.setParticleRadius(tRadius);
	thrustEmitter.setPosition(lander.model.getPosition() + glm::vec3(0, 1, 0));
	thrustEmitter.update();

	if (bThrustEmit) thrustEmitter.setRate(tRate); else thrustEmitter.setRate(0);

	for (ParticleForce* force : thrustEmitter.sys->forces) {
		if (dynamic_cast<GravityForce*>(force)) {
			dynamic_cast<GravityForce*>(force)->set(ofVec3f(0, -tGravity, 0));
		}
		else if (dynamic_cast<TurbulenceForce*>(force)) {
			dynamic_cast<TurbulenceForce*>(force)->set(static_cast<ofVec3f>(tTurbulenceMin), static_cast<ofVec3f>(tTurbulenceMax));
		}
	}

	// explosionEmitter
	ofSeedRandom();
	explosionEmitter.setPosition(lander.model.getPosition() + glm::vec3(0, 2, 0));
	for (ParticleForce* force : explosionEmitter.sys->forces) {
		if (dynamic_cast<GravityForce*>(force)) {
			dynamic_cast<GravityForce*>(force)->set(ofVec3f(0, -eGravity, 0));
		}
		else if (dynamic_cast<TurbulenceForce*>(force)) {
			dynamic_cast<TurbulenceForce*>(force)->set(static_cast<ofVec3f>(eTurbulenceMin), static_cast<ofVec3f>(eTurbulenceMax));
		}
		else if (dynamic_cast<ImpulseRadialForce*>(force)) {
			dynamic_cast<ImpulseRadialForce*>(force)->set(radialForceVal);
		}
	}
	for (Particle p : explosionEmitter.sys->particles) {
		p.damping = eDamping;
	}
	explosionEmitter.setLifespan(eLifespan);
	explosionEmitter.setVelocity(ofVec3f(eVelocity->x, eVelocity->y, eVelocity->z));
	explosionEmitter.setRate(eRate);
	explosionEmitter.setParticleRadius(eRadius);
	explosionEmitter.update();

	if (theCam == &trackingCam) {
		theCam->lookAt(lander.model.getPosition());
	} else if (theCam == &mountedCam) {
		theCam->lookAt(glm::vec3(lander.model.getPosition().x, lander.model.getPosition().y - 1,lander.model.getPosition().z));
		theCam->setPosition(lander.model.getPosition() + glm::vec3(1, 12, 1));
	}

	if (start) lander.integrate();
	//cout << lander.angularVel << "\n";
	//cout << altitude << '\n';
}
//--------------------------------------------------------------
void ofApp::draw() {
	ofBackground(ofColor::black);

	theCam->begin();
	ofPushMatrix();
	if (bWireframe) {                    // wireframe mode  (include axis)
		ofDisableLighting();
		ofSetColor(ofColor::slateGray);
		mars.drawWireframe();
		if (true) {
			lander.model.drawWireframe();
			if (!bTerrainSelected) drawAxis(lander.model.getPosition());
		}
		if (bTerrainSelected) drawAxis(ofVec3f(0, 0, 0));
	}
	else {
		ofEnableLighting();              // shaded mode
		mars.drawFaces();
		lava.drawFaces();
		skybox.drawFaces();

		// marked landing zones
		ofFill();
		ofSetColor(ofColor::cyan);
		ofDrawSphere(glm::vec3(57.6471, 17.7934, -38.8235), 5.0); // 1
		ofDrawSphere(glm::vec3(-74.7059, 17.9984,-108.235), 5.0); // 2
		ofDrawSphere(glm::vec3(26.4706, 18.111, 99.4118), 5.0); // 3

		// preview light positions
		//for (ofLight l : lights) {
		//	ofSetColor(l.getDiffuseColor());
		//	l.draw();
		//}

		ofSetColor(ofColor::white);
		lander.model.drawFaces();

		// thrust particle emitter
		thrustEmitter.draw();

		// explosion particle emitter
		if (bRadialExplosion)
			explosionEmitter.draw();

		ofMesh mesh;
		if (true) {
			//lander.model.drawFaces();
			if (!bTerrainSelected) drawAxis(lander.model.getPosition());
			if (bDisplayBBoxes) {
				ofNoFill();
				ofSetColor(ofColor::white);
				for (int i = 0; i < lander.model.getNumMeshes(); i++) {
					ofPushMatrix();
					ofMultMatrix(lander.model.getModelMatrix());
					ofRotate(-90, 1, 0, 0);
					Octree::drawBox(bboxList[i]);
					ofPopMatrix();
				}
			}

			if (bLanderSelected) {

				ofVec3f min = lander.model.getSceneMin() + lander.model.getPosition();
				ofVec3f max = lander.model.getSceneMax() + lander.model.getPosition();

				Box bounds = Box(Vector3(min.x, min.y, min.z), Vector3(max.x, max.y, max.z));
				ofNoFill();
				ofSetColor(ofColor::white);
				Octree::drawBox(bounds);

				// draw colliding boxes
				//
				ofNoFill();
				ofSetColor(ofColor::lightBlue);
				for (int i = 0; i < colBoxList.size(); i++) {
					Octree::drawBox(colBoxList[i]);
				}
			}
		}
	}
	if (bTerrainSelected) drawAxis(ofVec3f(0, 0, 0));

	// recursively draw octree
	//
	ofDisableLighting();
	int level = 0;
	//	ofNoFill();

	if (bDisplayLeafNodes) {
		ofNoFill();

		ofSetColor(ofColor::red);
		octree.drawLeafNodes(octree.root);
		cout << "num leaf: " << octree.numLeaf << endl;
    }
	else if (bDisplayOctree) {
		ofNoFill();
		ofSetColor(ofColor::white);
		octree.draw(numLevels, 0);
	}

	// if point selected, draw a sphere
	//

	ofPopMatrix();
  
	theCam->end();

	glDepthMask(false);
	if (!bHide) gui.draw();
	glDepthMask(true);

	ofSetColor(ofColor::white);
	verdana20.drawString(fuelString, ofGetWidth() - verdana20.stringWidth(fuelString) - 20, 2 * verdana20.stringHeight(fuelString));
	verdana20.drawString(heightString, ofGetWidth() - verdana20.stringWidth(heightString) - 20, 4 * verdana20.stringHeight(heightString));
}


// 
// Draw an XYZ axis in RGB at world (0,0,0) for reference.
//
void ofApp::drawAxis(ofVec3f location) {

	ofPushMatrix();
	ofTranslate(location);

	ofSetLineWidth(1.0);

	// X Axis
	ofSetColor(ofColor(255, 0, 0));
	ofDrawLine(ofPoint(0, 0, 0), ofPoint(1, 0, 0));
	

	// Y Axis
	ofSetColor(ofColor(0, 255, 0));
	ofDrawLine(ofPoint(0, 0, 0), ofPoint(0, 1, 0));

	// Z Axis
	ofSetColor(ofColor(0, 0, 255));
	ofDrawLine(ofPoint(0, 0, 0), ofPoint(0, 0, 1));

	ofPopMatrix();
}


void ofApp::keyPressed(int key) {

	switch (key) {
	case 'B':
	case 'b':
		bDisplayBBoxes = !bDisplayBBoxes;
		break;
	case 'C':
	case 'c':
		if (cam.getMouseInputEnabled()) cam.disableMouseInput();
		else cam.enableMouseInput();
		break;
	case 'F':
	case 'f':
		ofToggleFullscreen();
		break;
	case 'H':
	case 'h':
		bHide = !bHide;
		break;
	case 'L':
	case 'l':
		bDisplayLeafNodes = !bDisplayLeafNodes;
		break;
	case 'O':
	case 'o':
		bDisplayOctree = !bDisplayOctree;
		break;
	case 'r':
		cam.reset();
		break;
	case 's':
		savePicture();
		break;
	case 't':
		isTarget = !isTarget;
		setCameraTarget();
		break;
	case 'u':
		if (colBoxList.size() >= 10) {
			glm::vec3 landerPos = lander.model.getPosition();
			lander.model.setPosition(landerPos.x, landerPos.y + 1, landerPos.z);

			ofVec3f min = lander.model.getSceneMin() + lander.model.getPosition();
			ofVec3f max = lander.model.getSceneMax() + lander.model.getPosition();

			Box bounds = Box(Vector3(min.x, min.y, min.z), Vector3(max.x, max.y, max.z));
			colBoxList.clear();
			octree.intersect(bounds, octree.root, colBoxList);
		}
		break;
	case 'v':
		togglePointsDisplay();
		break;
	case 'V':
		break;
	case ' ':
		bThrustEmit = true;
		break;
	case 'x':
		explosionEmitter.sys->reset();
		explosionEmitter.start();
		break;
	case 'w':
		toggleWireframeMode();
		break;
	case OF_KEY_ALT:
		cam.enableMouseInput();
		bAltKeyDown = true;
		break;
	case OF_KEY_CONTROL:
		bCtrlKeyDown = true;
		break;
	case OF_KEY_SHIFT:
		break;
	case OF_KEY_DEL:
		break;
	case OF_KEY_F1:
		theCam = &cam;
		break;
	case OF_KEY_F2:
		theCam = &trackingCam;
		break;
	case OF_KEY_F3:
		theCam = &mountedCam;
	case OF_KEY_LEFT:   // turn left
	{
		ofMatrix4x4 rotationMatrix = lander.model.getModelMatrix();
		ofVec3f rightVector = rotationMatrix.getRowAsVec3f(0);
		rightVector.normalize();
		lander.force += rightVector * glm::vec3(-1, 0, -1);
		break;
	}
	case OF_KEY_RIGHT:  // turn right
	{
		ofMatrix4x4 rotationMatrix = lander.model.getModelMatrix();
		ofVec3f rightVector = rotationMatrix.getRowAsVec3f(0);
		rightVector.normalize();
		lander.force += rightVector * glm::vec3(1, 0, 1);
		break;
	}
	case OF_KEY_UP:     // go forward
		lander.force += lander.heading * glm::vec3(0, 1, 0);
		break;
	case OF_KEY_DOWN:   // go backward
		lander.force += lander.heading * glm::vec3(0, -1, 0);
		break;
	case 'e':
		lander.angularForce += 50;
		break;
	case 'q':
		lander.angularForce += -50;
		break;
	case 'z':
		start = !start;
		break;
	default:
		break;
	}
}

void ofApp::toggleWireframeMode() {
	bWireframe = !bWireframe;
}

void ofApp::toggleSelectTerrain() {
	bTerrainSelected = !bTerrainSelected;
}

void ofApp::togglePointsDisplay() {
	bDisplayPoints = !bDisplayPoints;
}

void ofApp::keyReleased(int key) {

	switch (key) {
	case ' ':
		bThrustEmit = false;
		break;
	case OF_KEY_ALT:
		cam.disableMouseInput();
		bAltKeyDown = false;
		break;
	case OF_KEY_CONTROL:
		bCtrlKeyDown = false;
		break;
	case OF_KEY_SHIFT:
		break;
	default:
		break;

	}
}



//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

	
}


//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button) {

	// if moving camera, don't allow mouse interaction
	//
	if (cam.getMouseInputEnabled()) return;

	// if moving camera, don't allow mouse interaction
//
	if (cam.getMouseInputEnabled()) return;

	// if rover is loaded, test for selection
	//
	if (true) {
		glm::vec3 origin = cam.getPosition();
		glm::vec3 mouseWorld = cam.screenToWorld(glm::vec3(mouseX, mouseY, 0));
		glm::vec3 mouseDir = glm::normalize(mouseWorld - origin);

		ofVec3f min = lander.model.getSceneMin() + lander.model.getPosition();
		ofVec3f max = lander.model.getSceneMax() + lander.model.getPosition();

		Box bounds = Box(Vector3(min.x, min.y, min.z), Vector3(max.x, max.y, max.z));
		bool hit = bounds.intersect(Ray(Vector3(origin.x, origin.y, origin.z), Vector3(mouseDir.x, mouseDir.y, mouseDir.z)), 0, 10000);
		if (hit) {
			bLanderSelected = true;
			mouseDownPos = getMousePointOnPlane(lander.model.getPosition(), cam.getZAxis());
			mouseLastPos = mouseDownPos;
			bInDrag = true;
		}
		else {
			bLanderSelected = false;
		}
	}
	else {
		ofVec3f p;
		raySelectWithOctree(p);
	}
}

bool ofApp::raySelectWithOctree(ofVec3f &pointRet) {
	ofVec3f mouse(mouseX, mouseY);
	ofVec3f rayPoint = cam.screenToWorld(mouse);
	ofVec3f rayDir = rayPoint - cam.getPosition();
	rayDir.normalize();
	Ray ray = Ray(Vector3(rayPoint.x, rayPoint.y, rayPoint.z),
		Vector3(rayDir.x, rayDir.y, rayDir.z));

	startTime = ofGetElapsedTimeMillis();
	pointSelected = octree.intersect(ray, octree.root, selectedNode);
	finalTime = ofGetElapsedTimeMillis() - startTime;
	//cout << finalTime << " ms\n";

	if (pointSelected) {
		pointRet = octree.mesh.getVertex(selectedNode.points[0]);

		// used for determining landing zone locations
		//cout << pointRet.x << ", " << pointRet.y << ", " << pointRet.z << "\n";
	}
	return pointSelected;
}




//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button) {

	// if moving camera, don't allow mouse interaction
	//
	if (cam.getMouseInputEnabled()) return;

	if (bInDrag) {


	}
	else {
		ofVec3f p;
		raySelectWithOctree(p);
	}
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button) {
	bInDrag = false;
}



// Set the camera to use the selected point as it's new target
//  
void ofApp::setCameraTarget() {
	if (theCam == &cam) {
		if (isTarget)
			theCam->lookAt(lander.model.getPosition());
		else
			theCam->lookAt(glm::vec3(0, 0, 0));
	}
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
// setup basic ambient lighting in GL  (for now, enable just 1 light)
//
void ofApp::initLightingAndMaterials() {

	static float ambient[] =
	{ .5f, .5f, .5, 1.0f };
	static float diffuse[] =
	{ 1.0f, 1.0f, 1.0f, 1.0f };

	static float position[] =
	{5.0, 5.0, 5.0, 0.0 };

	static float lmodel_ambient[] =
	{ 1.0f, 1.0f, 1.0f, 1.0f };

	static float lmodel_twoside[] =
	{ GL_TRUE };


	glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);
	glLightfv(GL_LIGHT0, GL_POSITION, position);

	glLightfv(GL_LIGHT1, GL_AMBIENT, ambient);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, diffuse);
	glLightfv(GL_LIGHT1, GL_POSITION, position);


	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, lmodel_ambient);
	glLightModelfv(GL_LIGHT_MODEL_TWO_SIDE, lmodel_twoside);

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
//	glEnable(GL_LIGHT1);
	glShadeModel(GL_SMOOTH);
} 

void ofApp::savePicture() {
	ofImage picture;
	picture.grabScreen(0, 0, ofGetWidth(), ofGetHeight());
	picture.save("screenshot.png");
	cout << "picture saved" << endl;
}
/*
//--------------------------------------------------------------
//
// support drag-and-drop of model (.obj) file loading.  when
// model is dropped in viewport, place origin under cursor
//
void ofApp::dragEvent2(ofDragInfo dragInfo) {

	ofVec3f point;
	mouseIntersectPlane(ofVec3f(0, 0, 0), cam.getZAxis(), point);
	if (lander.model.loadModel(dragInfo.files[0])) {
		lander.model.setScaleNormalization(false);
//		lander.model.setScale(.1, .1, .1);
	//	lander.model.setPosition(point.x, point.y, point.z);
		lander.model.setPosition(1, 1, 0);

		for (int i = 0; i < lander.model.getMeshCount(); i++) {
			bboxList.push_back(Octree::meshBounds(lander.model.getMesh(i)));
		}

		cout << "Mesh Count: " << lander.model.getMeshCount() << endl;
	}
	else cout << "Error: Can't load model" << dragInfo.files[0] << endl;
}
*/
bool ofApp::mouseIntersectPlane(ofVec3f planePoint, ofVec3f planeNorm, ofVec3f &point) {
	ofVec2f mouse(mouseX, mouseY);
	ofVec3f rayPoint = cam.screenToWorld(glm::vec3(mouseX, mouseY, 0));
	ofVec3f rayDir = rayPoint - cam.getPosition();
	rayDir.normalize();
	return (rayIntersectPlane(rayPoint, rayDir, planePoint, planeNorm, point));
}

//--------------------------------------------------------------
//
// support drag-and-drop of model (.obj) file loading.  when
// model is dropped in viewport, place origin under cursor
//
void ofApp::dragEvent(ofDragInfo dragInfo) {
	/*
	if (lander.model.loadModel(dragInfo.files[0])) {
		lander.model.setScaleNormalization(false);
		lander.model.setPosition(0, 0, 0);
		cout << "number of meshes: " << lander.model.getNumMeshes() << endl;
		bboxList.clear();
		for (int i = 0; i < lander.model.getMeshCount(); i++) {
			bboxList.push_back(Octree::meshBounds(lander.model.getMesh(i)));
		}

		//		lander.model.setRotation(1, 180, 1, 0, 0);

				// We want to drag and drop a 3D object in space so that the model appears 
				// under the mouse pointer where you drop it !
				//
				// Our strategy: intersect a plane parallel to the camera plane where the mouse drops the model
				// once we find the point of intersection, we can position the lander/lander
				// at that location.
				//

				// Setup our rays
				//
		glm::vec3 origin = cam.getPosition();
		glm::vec3 camAxis = cam.getZAxis();
		glm::vec3 mouseWorld = cam.screenToWorld(glm::vec3(mouseX, mouseY, 0));
		glm::vec3 mouseDir = glm::normalize(mouseWorld - origin);
		float distance;

		bool hit = glm::intersectRayPlane(origin, mouseDir, glm::vec3(0, 0, 0), camAxis, distance);
		if (hit) {
			// find the point of intersection on the plane using the distance 
			// We use the parameteric line or vector representation of a line to compute
			//
			// p' = p + s * dir;
			//
			glm::vec3 intersectPoint = origin + distance * mouseDir;

			// Now position the lander's origin at that intersection point
			//
			glm::vec3 min = lander.model.getSceneMin();
			glm::vec3 max = lander.model.getSceneMax();
			float offset = (max.y - min.y) / 2.0;
			lander.model.setPosition(intersectPoint.x, intersectPoint.y - offset, intersectPoint.z);

			// set up bounding box for lander while we are at it
			//
			landerBounds = Box(Vector3(min.x, min.y, min.z), Vector3(max.x, max.y, max.z));
		}
	}

	*/
}

//  intersect the mouse ray with the plane normal to the camera 
//  return intersection point.   (package code above into function)
//
glm::vec3 ofApp::getMousePointOnPlane(glm::vec3 planePt, glm::vec3 planeNorm) {
	// Setup our rays
	//
	glm::vec3 origin = cam.getPosition();
	glm::vec3 camAxis = cam.getZAxis();
	glm::vec3 mouseWorld = cam.screenToWorld(glm::vec3(mouseX, mouseY, 0));
	glm::vec3 mouseDir = glm::normalize(mouseWorld - origin);
	float distance;

	bool hit = glm::intersectRayPlane(origin, mouseDir, planePt, planeNorm, distance);

	if (hit) {
		// find the point of intersection on the plane using the distance 
		// We use the parameteric line or vector representation of a line to compute
		//
		// p' = p + s * dir;
		//
		glm::vec3 intersectPoint = origin + distance * mouseDir;

		return intersectPoint;
	}
	else return glm::vec3(0, 0, 0);
}
