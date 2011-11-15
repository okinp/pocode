#include "complexShapesApp.h"
#include "poApplication.h"
#include "poCamera.h"
#include "poHelpers.h"

#include "poResourceStore.h"
#include "poSimpleDrawing.h"
#include <iostream>

using namespace std;


// CLASS NOTES
//
// poShape2D derives directly from poObject and is the parent class of all 2D graphics objects
// in potionCode, including poRectShape, poOvalShape and poLineShape (see "poShapeBasics2D.h").
//
// All poShape2D objects and poShape2D subclasses:
//
// + Have all the properties and methods of poObject.
// + Have a list of "points" that form the contour of the shape.
// + Have fillColor and strokeColor properties
// + Have fillEnabled and strokeEnabled properties.
// + Have a fillDrawStyle property that determines how OpenGL uses the points (GL_TRIANGLE_STRIP, GL_POLYGON, etc)
// + Have a generateStroke method for generating a high-quality thick stroke with multiple capping options.
// + Have a placeTexture and and transformTexture methods for assigning a poTexture to the shape
// + Have a pointInside method for testing if a point is inside the shape.

poObject *createObjectForID(uint uid) {
	return new complexShapesApp();
}

void setupApplication() {
	
    
    // define path to the resource folder
    lookUpAndSetPath("resources");
    
    // define window settings
	applicationCreateWindow(0, WINDOW_TYPE_NORMAL, "MainWindow", 100, 100, 800, 600);
}

void cleanupApplication() {
}

complexShapesApp::complexShapesApp() {

	// define the 2D camera and set it's color
    addModifier(new poCamera2D(poColor(.2,.2,.2)));
    addEvent(PO_MOUSE_DOWN_EVENT, this);

    // add a poRectshape with a background image
    poRectShape* BG = new poRectShape("bg.jpg");
    addChild( BG );

    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //
    // make a heart shape, using a vector of points
    // find many more fun forumas to generate curves at http://mathworld.wolfram.com/topics/Curves.html   
    //
    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  
    poShape2D* heartShape = new poShape2D();
    std::vector<poPoint> points;

    for ( int i=0 ; i<=120 ; i++ )
    {   
        // set veriables to control the progression of the mathematical formula          
        float dirX,dirY;
        float scale = 0.05;
        int val;

        if (i<=60) 
        {
            dirX = -1;
            dirY = -1;
            val = i;
        } 
        
        if (i>60) 
        {
            dirX = 1;
            dirY = -1;
            val = (int)(i-60);
        } 
        
        // mathematical equation to calculat x and y of a point
        float x = dirX*scale*(-powf(val, 2)+40*val+1200)*sin(M_PI*val/180); 
        float y = dirY*scale*(-powf(val, 2)+40*val+1200)*cos(M_PI*val/180);
        poPoint P = poPoint( x,y );

        // add point to vector
        points.push_back( P );
		printf("%f %f\n", x, y);
    }
    
    // insert the points generated by the formula into the shape
    heartShape->addPoints( points );
    
    // set position and color
    heartShape->position.set( 300, 300, 0 );
    heartShape->fillColor = poColor(poColor::dkGrey);
    
    // add events to the shape created
    heartShape->addEvent(PO_MOUSE_ENTER_EVENT, this);
    heartShape->addEvent(PO_MOUSE_LEAVE_EVENT, this);
    addChild( heartShape );
    
    
    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //
    // make a scribbled line shape
    //
    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    
    // create a new po2Dshape 
    poShape2D* curvedShape = new poShape2D();

    //------------------------------------------------------------------------------------------------------------
    
    // add the first point
    curvedShape->addPoint( 0,0);

    // add points to the curve 
    for ( int i=1 ; i<20 ; i++) {
        
        float size = 20;
        
        // add some randomness to create a scribbly effect
        float rnd = poRand( 0.9,1.1 );
        size *= rnd;
        
        // define an end point and control point to describe the curve
        // alternatively a curve sergment can be defined using two control points: 
        // curveTo( poPoint pt, poPoint control1, poPoint control2 )
        
        if (i%2 == 0) {
            curvedShape->curveTo( poPoint(size*i,size/2), poPoint(i*size -size/2,size*2),100);
        } else {
        
            curvedShape->curveTo( poPoint(size*i,-size/2), poPoint(i*size - size/2,-size*2),100);
        }
    }

    // set position and rotation
    curvedShape->position = poPoint(100,200);
    curvedShape->rotation = 45;
    
    // these settings are used to define a stroked line
    curvedShape->strokeEnabled = true;
    curvedShape->strokeColor = poColor::black;
    curvedShape->fillEnabled = false;
    curvedShape->useSimpleStroke = true;
    curvedShape->closed = false;
    
    // add events to the shape created
    curvedShape->addEvent(PO_MOUSE_ENTER_EVENT, this);
    curvedShape->addEvent(PO_MOUSE_LEAVE_EVENT, this);
    
    // add the shape to the scene
    addChild( curvedShape );
    
    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //
    // importing from an SVG file
    //
    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    
    
    // import curves from an SVG file
    SVGShapes = createShapesFromSVGfile("test.svg");
    
    // iterate through the shapes in the shapes vector created
    for ( int i=0 ; i<SVGShapes.size() ; i++ ) {
        poShape2D* S = new poShape2D();
        S = SVGShapes[i];
        
        // define the appearence the shape
        S->useSimpleStroke = true;
        S->strokeEnabled = true;
        S->fillEnabled = false;
        S->fillColor = poColor::cyan;
        
        // add events to the shape
        S->addEvent(PO_MOUSE_ENTER_EVENT, this);
        S->addEvent(PO_MOUSE_LEAVE_EVENT, this);
        
        /*
        // print out a list of shapes and their respective points
        cout << "shape: " << i << ":\n";
        for ( int j=0 ; j<S->points.size() ; j++ ) 
        {
            cout << "point " << j << ": "<< S->points[j] << endl; 
        }
        */
        
        // add the shape to the scene       
        addChild( S );
    }

}

complexShapesApp::~complexShapesApp() {
}


void complexShapesApp::eventHandler(poEvent *event) {
    
    // define what happens when an event is fired
    
    if (event->type == PO_MOUSE_ENTER_EVENT) {
        
        // set parameters for the shape in which the event occured
        poShape2D* S = (poShape2D*)event->source;
        S->fillColor = poColor::red;
        S->strokeColor = poColor::red;
        
    }
    
    if (event->type == PO_MOUSE_LEAVE_EVENT) {
        // set parameters for the shape in which the event occured
        poShape2D* S = (poShape2D*)event->source;
        S->fillColor = poColor::ltGrey;
        S->strokeColor = poColor::black;
    }
}




