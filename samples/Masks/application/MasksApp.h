/*	Created by Joshua Fisher on 6/2/11.
 *	Copyright 2011 Potion Design. All rights reserved.
 */

#include "poObject.h"

class MasksApp : public poObject {
public:
	MasksApp();
	virtual void eventHandler(poEvent *event);
	
	poObject *holder;
	
	void setupAperture();
	
	void start();
	void stop();
	void open();
	void close();
};