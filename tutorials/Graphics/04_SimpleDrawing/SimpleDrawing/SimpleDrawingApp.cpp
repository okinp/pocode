/////////////////////////////////////////
//
// LESSON : Simple drawing
//
/////////////////////////////////////////

#include "SimpleDrawingApp.h"
#include "poApplication.h"
#include "poCamera.h"
#include "poShapeBasics2D.h"
#include "poSimpleDrawing.h"


// APP CONSTRUCTOR.
// Create all objects here.
SimpleDrawingApp::SimpleDrawingApp() {
	
	// Add a camera
	addModifier(new poCamera2D(poColor::black));
	
	// Show poCode lesson image in the background
    poRectShape* BG = new poRectShape("bg.jpg");
    BG->alpha = 0.5;
//	addChild( BG );
	
	bgTex = new poTexture("bg.jpg");
	rectTex = new poTexture("pear.jpg");
	polygonTex = new poTexture("orange.jpg");
}


// APP DESTRUCTOR
// Delete all objects here. (optional)
SimpleDrawingApp::~SimpleDrawingApp() {
}


// DRAW
// Draw directly here. This is called to draw everything.
void SimpleDrawingApp::draw() {
	
	po::setColor(poColor::white);
	po::drawTexturedRect(bgTex, 0, 0, 800, 600);
	
	
	// INTERNAL!! generateStroke not working
//	po::generateStroke(points, 10);
	
	
	// A. Draw lines ///////////////////////
	
	float Ax, Ay, Bx, By;
	for (int i=0; i < (150/2.5); i++) {
		
		if (i < (150/5)) {
			Ax = 80;
			Ay = 230 + (5*i);
			Bx = 80 + (5*i);
			By = 230;
		}
		else {
			Ax = 80 + (5*(i - 150/5));
			Ay = 380;
			Bx = 230;
			By = 230 + (5*(i - 150/5));
		}
		
		po::setColor(poHSVColor(1/(150/2.5) + i * (1/(150/2.5)), 1, 1));
		po::drawLine(poPoint(Ax, Ay), poPoint(Bx, By));
	}
	
	
	// B. Draw rectangles ///////////////////////
	
	po::setColor(poColor(0.2, 0.6, 0.2));
	
	po::drawStrokedRect(305, 210, 120, 120);
	
	po::drawFilledRect(325, 240, 120, 120);
	
	po::setColor(poColor::white);
	po::drawTexturedRect(rectTex, 345, 270, 160, 120);
	
	
	// C. Draw polygons ///////////////////////
	
	po::setColor(poColor::orange);
	
	poPoint A(620, 210);
	poPoint B(700, 270);
	poPoint C(670, 350);
	poPoint D(560, 330);
	poPoint E(545, 270);
	
	std::vector<poPoint> points;
	points.push_back(A);
	points.push_back(B);
	points.push_back(C);
	points.push_back(D);
	points.push_back(E);
	
	po::drawStrokedPolygon(points);
	
	for(int i=0; i < points.size(); i++) {
		points[i] += poPoint(20,30);
	}
	
	po::drawFilledPolygon(points);
	
	for(int i=0; i < points.size(); i++) {
		points[i] += poPoint(20,30);
	}
	
	float leftMost = MAXFLOAT;
	float rightMost = FLT_MIN;
	float topMost = MAXFLOAT;
	float bottomMost = FLT_MIN;
	for(int i=0; i < points.size(); i++) {
		if(points[i].x < leftMost) leftMost = points[i].x;
		if(points[i].x > rightMost) rightMost = points[i].x;
		if(points[i].y < topMost) topMost = points[i].y;
		if(points[i].y > bottomMost) bottomMost = points[i].y;
	}
	poRect polygonBounds(leftMost, topMost, rightMost - leftMost, bottomMost - topMost);
//	polygonBounds.width = polygonTex->getWidth();
//	polygonBounds.height = polygonTex->getHeight();
	
	std::vector<poPoint> texCoords;
	for(int i=0; i < points.size(); i++) {
		
		float coordX = (points[i].x - leftMost) / polygonBounds.width;
		float coordY = 1 - (points[i].y - topMost) / polygonBounds.height;
		
		texCoords.push_back(poPoint(coordX, coordY));
	}
	
	po::setColor(poColor::white);
	po::drawTexturedPolygon(points, polygonTex, texCoords);
}


// UPDATE
// Animate objects here. This is called after every frame is drawn.
void SimpleDrawingApp::update() {
}


// EVENT HANDLER
// Respond to user events here.
void SimpleDrawingApp::eventHandler(poEvent *event) {
}


// MESSAGE HANDLER
// Receive inter-object messages here.
void SimpleDrawingApp::messageHandler(const std::string &msg, const poDictionary& dict) {
}
