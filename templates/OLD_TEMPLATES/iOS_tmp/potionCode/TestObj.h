//
//  TestObj.h
//  potionCode
//
//  Created by Joshua Fisher on 9/21/11.
//  Copyright 2011 Potion Design. All rights reserved.
//

#pragma once
#include "poObject.h"
#include "poTextBox.h"

class TestObj : public poObject {
public:
	TestObj();
	
	void draw();
    void eventHandler(poEvent *event);
	
};