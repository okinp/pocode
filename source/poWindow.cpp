#include "poObject.h"
#include "poWindow.h"

#include "Helpers.h"
#include "poOpenGLState.h"
#include "poApplication.h"

void objUnderPoint(poObject *obj, poPoint &pnt, std::set<poObject*> &objsBeneath) {
	if(!(obj->visible && obj->alpha > 0.01))
		return; 
	
	for(int i=obj->getNumChildren()-1; i>=0; i--) {
		objUnderPoint(obj->getChild(i), pnt, objsBeneath);
	}

	if(obj->pointInside(pnt,true))
		objsBeneath.insert(obj);
}

poWindow::poWindow(const char *title, uint rootID, poRect b)
:	title(title)
,	handle(NULL)
,	root(NULL)
,	rootID(rootID)
,	bounds(b)
//,   key_receiver(NULL)
,	fullscreen(false)
,	closed(false)
,	framecounter(0)
,	totalFramecount(0)
,	lastMark(0.0)
,	framerate(0.f)
{
}

poWindow::~poWindow() {
	makeCurrent();
	delete root;
}

void poWindow::moveTo(poPoint p) {
	applicationMoveWindow(this, p);
}

void poWindow::setFullscreen(bool b) {
	fullscreen = b;
	applicationMakeWindowFullscreen(this, b);
}

int poWindow::getX() const {
	return bounds.x;
}

std::string poWindow::getTitle() const {
	return title;
}

int poWindow::getY() const {
	return bounds.y;
}

int poWindow::getWidth() const {
	return bounds.width;
}

poPoint poWindow::getDimensions() const {
	return bounds.getSize();
}

poRect poWindow::getFrame() const {
	return bounds;
}

poRect poWindow::getBounds() const {
	return poRect(poPoint(0,0), bounds.getSize());
}

poPoint poWindow::getCenterPoint() const {
	return poPoint(bounds.width/2.f, bounds.height/2.f);
}

int poWindow::getHeight() const {
	return bounds.height;
}

float poWindow::getFramerate() const {
	return framerate;
}

int poWindow::getFramecount() const {
	return totalFramecount;
}

float poWindow::getLastFrameElapsed() const {
	return lastElapsed;
}

float poWindow::getLastFrameTime() const {
	return lastFrame;
}

bool poWindow::isFullscreen() const {
	return fullscreen;
}

poObject *poWindow::getRootObject() {
	if(!root) {
		makeCurrent();
		poOpenGLState::get()->setBlend(po::BlendState::preMultipliedBlending());
		root = createObjectForID(rootID);
	}
	return root;
}

poPoint poWindow::getMousePosition() const {
	return mousePos;
}

void poWindow::makeCurrent() {
	applicationMakeWindowCurrent(this);
}

void poWindow::draw() {
	drawOrderCounter = 0;
    poEventCenter::get()->negateDrawOrderForObjectWithEvents();
	getRootObject()->drawTree();
}

void poWindow::update() {
	double now = getTime();
	
	totalFramecount++;
	framecounter++;
	if(now - lastMark >= 1.0) {
		lastMark = now;
		framerate = framecounter;
		framecounter = 0;
	}
	
	lastElapsed = now - lastFrame;
	lastFrame = now;

	// handle events
	if(handle && !received.empty()) {
		poEventCenter::get()->processEvents(received);
	}
	received.clear();

	// update the objects
	getRootObject()->updateTree();
}

void poWindow::mouseDown(int x, int y, int mod) {
	mousePos.set(x,y,1);
	
	poEvent event;
	event.globalPosition.set(x, y, 0.f);
	event.modifiers = mod;
	
	event.type = PO_MOUSE_DOWN_EVENT;
	received.push_back(event);
}

void poWindow::mouseUp(int x, int y, int mod) {
	mousePos.set(x,y,1);
	
	poEvent event;
	event.globalPosition.set(x, y, 0.f);
	event.modifiers = mod;

	event.type = PO_MOUSE_UP_EVENT;
	received.push_back(event);
}

void poWindow::mouseMove(int x, int y, int mod) {
	mousePos.set(x,y,1);
	
	poEvent event;
	event.globalPosition.set(x, y, 0.f);
	event.modifiers = mod;
	
	event.type = PO_MOUSE_MOVE_EVENT;
	received.push_back(event);
}

void poWindow::mouseDrag(int x, int y, int mod) {
	mousePos.set(x,y,1);
	
	poEvent event;
	event.globalPosition.set(x, y, 0.f);
	event.modifiers = mod;
	
	event.type = PO_MOUSE_DRAG_EVENT;
	received.push_back(event);
}

void poWindow::mouseWheel(int x, int y, int mod, int num_steps) {
}

void poWindow::keyDown(int key, int code, int mod) {
	poEvent event;
	event.keyChar = key;
	event.keyCode = code;
	event.modifiers = mod;
	
	event.type = PO_KEY_DOWN_EVENT;
	received.push_back(event);
}

void poWindow::keyUp(int key, int code, int mod) {
	poEvent event;
	event.keyCode = code;
	event.keyChar = key;
	event.modifiers = mod;
	
	event.type = PO_KEY_UP_EVENT;
	received.push_back(event);
}


void poWindow::resized(int w, int h) {
	resized(bounds.x, bounds.y, w, h);
}


void poWindow::resized(int x, int y, int w, int h) {
	bounds.set(x,y,w,h);
//
//	poEvent event;
//	event.type = PO_WINDOW_RESIZED_EVENT;
//	received.push_back(event);
}


void poWindow::touchBegin(int x, int y, int uid, int tapCount ) {
    //Create an interactionPoint for this touch, with a unique id
    interactionPoint *t = new interactionPoint();
    t->uid = uid;
    t->bIsDead = false;
    
    //Begin tracking touch + give it a simple ID (0-100)
    trackTouch(t);
    
    //Fire Event
	poEvent event;
	event.globalPosition.set(x, y, 0.f);
	event.touchID   = t->id;
    event.uniqueID  = uid;
    event.tapCount  = tapCount;
	
	event.type = PO_TOUCH_BEGAN_EVENT;
	received.push_back(event);
}


void poWindow::touchMove(int x, int y, int uid, int tapCount ) {
    //Get the corresponding tracked object
    interactionPoint *t = getTouch(uid);
    
    //Send event
	poEvent event;
	event.globalPosition.set(x, y, 0.f);
	event.touchID   = t->id;
    event.uniqueID  = uid;
    event.tapCount  = tapCount;
	
	event.type = PO_TOUCH_MOVED_EVENT;
	received.push_back(event);
}


void poWindow::touchEnd(int x, int y, int uid, int tapCount ) {
    //Get the corresponding tracked object
    interactionPoint *t = getTouch(uid);
   
    //Send event
	poEvent event;
	event.globalPosition.set(x, y, 0.f);
	event.touchID   = t->id;
    event.uniqueID  = uid;
    event.tapCount = tapCount;
	
	event.type = PO_TOUCH_ENDED_EVENT;
    received.push_back(event);
    
    untrackTouch(uid);
}


void poWindow::touchCancelled(int x, int y, int uid, int tapCount ) {
    untrackTouch(uid);
    
    poEvent event;
	event.globalPosition.set(x, y, 0.f);
	event.touchID = uid;
    event.tapCount = tapCount;
	
	event.type = PO_TOUCH_CANCELLED_EVENT;
	received.push_back(event);
    
    untrackTouch(uid);
}


void poWindow::trackTouch(interactionPoint *t) {
    int totalTouches = trackedTouches.size();
        
    //See if there are any empty slots
    for(int i=0; i<totalTouches; i++) {
        if(trackedTouches[i]->bIsDead) {
            //Delete old touch
            delete trackedTouches[i];
            
            //Set id
            t->id = i;
        
            //Track in this spot
            trackedTouches[i] = t;
            return;
        }
    }
    
    //If the touch wasn't found, add it
    t->id = trackedTouches.size();
    trackedTouches.push_back(t);
}


interactionPoint *poWindow::getTouch(int uid) {
    for(int i=0;i<trackedTouches.size(); i++) {
        if(trackedTouches[i]->uid == uid) {
            return trackedTouches[i];
        }
    }
}


void poWindow::untrackTouch(int uid) {
    interactionPoint *t = getTouch(uid);
    t->bIsDead = true;
}

void *poWindow::getWindowHandle() {
	return handle;
}

void poWindow::setWindowHandle(void *handle) {
	this->handle = handle;
}

int poWindow::getNextDrawOrder() {
	return drawOrderCounter++;
}
