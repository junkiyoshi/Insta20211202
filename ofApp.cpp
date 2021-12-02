#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {

	ofSetFrameRate(60);
	ofSetWindowTitle("openframeworks");

	ofBackground(0);
	//ofEnableBlendMode(ofBlendMode::OF_BLENDMODE_ADD);
	ofSetLineWidth(1);

	this->gon = 10;
	for (auto i = 0; i < this->gon; i++) { this->gap_list.push_back(0); }
}

//--------------------------------------------------------------
void ofApp::update() {

	ofSeedRandom(39);

	if (ofGetFrameNum() % 120 < 45) {

		this->noise_param += ofMap(ofGetFrameNum() % 60, 0, 45, 0.045, 0.001);
	}
	else {

		this->noise_param += 0.001;
	}

	for (auto& gap : this->gap_list) {

		gap = ofMap(ofNoise(ofRandom(1000), this->noise_param), 0, 1, 0.01, 0.5);
	}
}

//--------------------------------------------------------------
void ofApp::draw() {

	ofPushMatrix();
	ofTranslate(ofGetWindowSize() * 0.5);
	ofRotate(this->noise_param * RAD_TO_DEG * 10);

	auto radius = 600;
	int deg_span = 360 / this->gon;
	vector<glm::vec2> vertices;
	auto first = true;
	while (true) {

		if (first) {

			for (auto deg = 0; deg < 360; deg += deg_span) {

				vertices.push_back(glm::vec2(radius * cos(deg * DEG_TO_RAD), radius * sin(deg * DEG_TO_RAD)));
			}

			first = false;
		}
		else {

			vector<glm::vec2> tmp;
			for (auto k = 0; k < vertices.size(); k++) {

				tmp.push_back(vertices[k] + (vertices[(k + 1) % vertices.size()] - vertices[k]) * this->gap_list[k]);
			}
			vertices = tmp;
		}

		ofSetColor(ofMap(glm::distance(vertices[0], vertices[1]), 300, 1, 0, 255));
		ofNoFill();
		ofBeginShape();
		ofVertices(vertices);
		ofEndShape(true);

		if (glm::distance(vertices[0], vertices[1]) < 1) {

			break;
		}
	}

	ofPopMatrix();
}

//--------------------------------------------------------------
int main() {

	ofSetupOpenGL(720, 720, OF_WINDOW);
	ofRunApp(new ofApp());
}