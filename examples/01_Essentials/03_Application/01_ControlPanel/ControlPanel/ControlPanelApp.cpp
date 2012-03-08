/*	Created by filippo on 2/24/12.
 *	Copyright 2012 __MyCompanyName__. All rights reserved.
 */

#include "ControlPanelApp.h"
#include "poApplication.h"
#include "poCamera.h"
#include "poImageShape.h"


// APP CONSTRUCTOR. Create all objects here.
ControlPanelApp::ControlPanelApp() {
	
	// Add a camera
	addModifier(new poCamera2D(poColor::black));
	
	// poCode template image in the background
    poImageShape* BG = new poImageShape("bg.jpg");
    addChild( BG );
	
	control = new poControlPanel( "controlPanel", poColor(0,0,0,0.6) );
    control->addToggle( "visible", this );
    control->addSliderF( "scale",0.5,5, this );
    control->addPointSlider( "loc", poPoint(50,172), poPoint(508,467), this );
    control->addColorSlider( "color", true, this );
    control->addKnob( "knob",0, 360, this );
	
	std::vector<std::string> radio_options;
	radio_options.push_back("triangle");
	radio_options.push_back("quadrilateral");
	radio_options.push_back("pentagon");
	radio_options.push_back("hexagon");
	radio_options.push_back("octagon");
	radio_options.push_back("circle");
	control->addRadio("shape", radio_options, this);
	
	control->addInputTextBox( "text",this );
	control->addSliderI( "size",10,30, this );
	
	shape = new poOvalShape(40,40,3);
	shape->fillColor = control->getColor("color");
	shape->position = control->getPoint("loc");
	shape->scale.set(control->getFloat("scale"),control->getFloat("scale"),1);
	shape->visible = control->getBool("visible");
	shape->rotation = control->getInt("knob");
	
	int numberVectors;
	int optionID = control->getInt("shape");
	if(optionID == 0) {
		numberVectors = 3;
	}
	else if(optionID == 1) {
		numberVectors = 4;
	}
	else if(optionID == 2) {
		numberVectors = 5;
	}
	else if(optionID == 3) {
		numberVectors = 6;
	}
	else if(optionID == 4) {
		numberVectors = 8;
	}
	else if(optionID == 5) {
		numberVectors = 40;
	}
	shape->reshape(40,40,numberVectors);
	
	text = new poTextBox(450,295);
	text->setText(control->getString("text"));
	text->setFont(poGetFont("Helvetica", "Bold"));
	text->setTextSize(control->getInt("size"));
	text->textColor = poColor::black;
	text->setTextAlignment(PO_ALIGN_CENTER_CENTER);
	text->doLayout();
	text->position.set(50, 172, 0);
	
	addChild(shape);
	addChild(text);
	addChild(control);
}

// APP DESTRUCTOR. Delete all objects here.
ControlPanelApp::~ControlPanelApp() {
}

// UPDATE. Called once per frame. Animate objects here.
void ControlPanelApp::update() {
//	poControl* C = (poControl*) control->container->getChild("alpha");
//	printf("valF %f\n", C->valF);
}

// DRAW. Called once per frame. Draw objects here.
void ControlPanelApp::draw() {
	
}

// EVENT HANDLER. Called when events happen. Respond to events here.
void ControlPanelApp::eventHandler(poEvent *event) {
	
}

// MESSAGE HANDLER. Called from within the app. Use for message passing.
void ControlPanelApp::messageHandler(const std::string &msg, const poDictionary& dict) {
	
	if ( msg == "loc" ) {
        shape->position = control->getPoint("loc");
    }
	
	if ( msg == "scale" ) {
        shape->scale.set(control->getFloat("scale"), control->getFloat("scale"), 1);
    }
	
	if ( msg == "color" ) {
        shape->fillColor = control->getColor("color");
    }
	
	if ( msg == "visible" ) {
        shape->visible = control->getBool("visible");
    }
	
	if ( msg == "knob" ) {
		shape->rotation = control->getFloat("knob");
	}
	
	if ( msg == "shape" ) {
		int numberVectors;
		int optionID = control->getInt("shape");
		if(optionID == 0) {
			numberVectors = 3;
		}
		else if(optionID == 1) {
			numberVectors = 4;
		}
		else if(optionID == 2) {
			numberVectors = 5;
		}
		else if(optionID == 3) {
			numberVectors = 6;
		}
		else if(optionID == 4) {
			numberVectors = 8;
		}
		else if(optionID == 5) {
			numberVectors = 40;
		}
		shape->reshape(40,40,numberVectors);
	}
	
	if ( msg == "text" ) {
		text->setText(control->getString("text"));
		text->doLayout();
	}
	
	if ( msg == "size" ) {
        text->setTextSize(control->getInt("size"));
		text->doLayout();
    }
}
