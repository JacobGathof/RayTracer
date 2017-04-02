#pragma once
#include "GenVector.h"


class Light {

public:
	Vector3 ka, kd, ks;
	Vector3 position;

	Light(Vector3 p, Vector3 a, Vector3 d, Vector3 s) {
		position = p;
		ka = a;
		kd = d;
		ks = s;
	}

};



