#pragma once
#include "GenVector.h"

class Material {

public:

	Vector3 ka, kd, ks;
	float r;

	Material(Vector3 a, Vector3 d, Vector3 s, float re=0) {
		ka = a;
		kd = d;
		ks = s;
		r = re;
	}


};
