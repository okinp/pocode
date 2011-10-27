//
//  poDictionary_test.cpp
//  potionCode
//
//  Created by filippo on 10/19/11.
//  Copyright 2011 Potion Design. All rights reserved.
//

#include <boost/test/unit_test.hpp>
#include "poObject.h"
#include "poDictionary.h"
#include "poImage.h"


BOOST_AUTO_TEST_CASE( poDictionaryTest ) {
	
	poDictionary D = poDictionary();
	
	bool b = true;
	D.set("bool", b);
	BOOST_CHECK(D.getBool("bool") == true);
	BOOST_CHECK(D.getInt("bool") == 1);
	D.set("bool", false);
	BOOST_CHECK(D.getBool("bool") == false);
	BOOST_CHECK(D.getInt("bool") == 0);
	b = D.getBool("bool");
	BOOST_CHECK(b == false);
	b = D.getInt("bool");
	BOOST_CHECK(b == false);
	
	int i = 1000000;
	D.set("int", i);
	BOOST_CHECK(D.getInt("int") == 1000000);
	D.set("int", -97698948);
	BOOST_CHECK(D.getInt("int") == -97698948);
	i = D.getInt("int");
	BOOST_CHECK(i == -97698948);
	
	float f = 1000000.000000f;
	D.set("float", f);
	BOOST_CHECK(D.getDouble("float") == 1000000.000000f);
	D.set("float", 999999.000000f);
	BOOST_CHECK(D.getDouble("float") == 999999.000000f);
	f = (float)D.getDouble("float");
	BOOST_CHECK(f == 999999.000000f);
	
	std::string str;
	D.set("string", str);
	BOOST_CHECK(D.getString("string").empty() == true);
	D.set("string", "string");
	BOOST_CHECK(D.getString("string") == "string");
	str = D.getString("string");
	BOOST_CHECK(str == "string");
	
	poColor C = poColor(1,1,1,1);
	D.set("color", C);
	BOOST_CHECK(D.getColor("color").R == 1);
	BOOST_CHECK(D.getColor("color").G == 1);
	BOOST_CHECK(D.getColor("color").B == 1);
	BOOST_CHECK(D.getColor("color").A == 1);
	D.set("color", poColor(2.5f,5.f,7.5f,5.f));
	BOOST_CHECK(D.getColor("color").R == 2.5f);
	BOOST_CHECK(D.getColor("color").G == 5.f);
	BOOST_CHECK(D.getColor("color").B == 7.5f);
	BOOST_CHECK(D.getColor("color").A == 5.f);
	C = D.getColor("color");
	BOOST_CHECK(C.R == 2.5f);
	BOOST_CHECK(C.G == 5.f);
	BOOST_CHECK(C.B == 7.5f);
	BOOST_CHECK(C.A == 5.f);
	
	poPoint P(1,1,1);
	D.set("point", P);
	BOOST_CHECK(D.getPoint("point").x == 1);
	BOOST_CHECK(D.getPoint("point").y == 1);
	BOOST_CHECK(D.getPoint("point").z == 1);
	D.set("point", poPoint(0.2f,0.5f,0.8f));
	BOOST_CHECK(D.getPoint("point").x == 0.2f);
	BOOST_CHECK(D.getPoint("point").y == 0.5f);
	BOOST_CHECK(D.getPoint("point").z == 0.8f);
	P = D.getPoint("point");
	BOOST_CHECK(P.x == 0.2f);
	BOOST_CHECK(P.y == 0.5f);
	BOOST_CHECK(P.z == 0.8f);
	
	poObject* obj;
	poObject* first_obj = new poObject();
	first_obj->name = "first object";
	poObject* second_obj = new poObject();
	second_obj->name = "second object";
	D.set("pointer", first_obj);
	obj = (poObject*) D.getPtr("pointer");
	BOOST_CHECK(obj == first_obj);
	BOOST_CHECK(obj->name == "first object");
	D.set("pointer", second_obj);
	obj = (poObject*) D.getPtr("pointer");
	BOOST_CHECK(obj == second_obj);
	BOOST_CHECK(obj->name == "second object");
	first_obj = (poObject*)D.getPtr("pointer");
	BOOST_CHECK(first_obj == second_obj);
	
//	poFont font = poFont("Lucida Grande");
//	D.set("font", font);
	
//	poImage img = poImage();
//	D.set("img", img);
	
	BOOST_CHECK(D.count() == 7);
	BOOST_CHECK(D.has("bool") == true);
	BOOST_CHECK(D.has("int") == true);
	BOOST_CHECK(D.has("float") == true);
	BOOST_CHECK(D.has("string") == true);
	BOOST_CHECK(D.has("color") == true);
	BOOST_CHECK(D.has("point") == true);
	BOOST_CHECK(D.has("pointer") == true);
	BOOST_CHECK(D.has("non-exhisting thing") == false);
	
	poDictionary Dcopy = D.copy();
	BOOST_CHECK(Dcopy.count() == 7);
	BOOST_CHECK(Dcopy.has("bool") == true);
	BOOST_CHECK(Dcopy.has("int") == true);
	BOOST_CHECK(Dcopy.has("float") == true);
	BOOST_CHECK(Dcopy.has("string") == true);
	BOOST_CHECK(Dcopy.has("color") == true);
	BOOST_CHECK(Dcopy.has("point") == true);
	BOOST_CHECK(Dcopy.has("pointer") == true);
	BOOST_CHECK(Dcopy.has("non-exhisting thing") == false);
	
	// change values in the copy
	Dcopy.set("string", "copied string");
	Dcopy.set("int", 987654);
	// add new item in the copy
	Dcopy.set("new item", 123456);
	
	D.append(Dcopy);
	BOOST_CHECK(D.count() == 8);
	BOOST_CHECK(D.has("new item") == true);
	BOOST_CHECK(D.getInt("new item") == 123456);
	BOOST_CHECK(D.getString("string") == "copied string");
	BOOST_CHECK(D.getInt("int") == 987654);
	
	Dcopy.set("another new item", "yeah");
	BOOST_CHECK(Dcopy.getString("another new item") == "yeah");
	BOOST_CHECK(D.has("another new item") == false);
	
	poDictionary newD = poDictionary();
	int num = 56373829;
	newD.set("number", num);
	D.set("dictionary", newD);
	BOOST_CHECK(num == D.getDictionary("dictionary").getInt("number"));
	poDictionary childD = D.getDictionary("dictionary");
	BOOST_CHECK(num == childD.getInt("number"));
}
