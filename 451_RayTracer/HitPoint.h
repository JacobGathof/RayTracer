#pragma once
#include "GenVector.h"
#include "Primitive.h"

class Primitive;

class HitPoint {

public:

	HitPoint(Primitive * obj, float t, bool hit = true) {
		this->object = obj;
		this->t = t;
		this->hit = hit;
	}

	Primitive * object;
	float t;
	bool hit;

};
