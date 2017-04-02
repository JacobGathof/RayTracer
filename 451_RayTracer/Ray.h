#pragma once
#include "GenVector.h"

class Ray {
private:
	Vector3 position, direction;
public:

	Ray() {}

	Ray(Vector3 pos, Vector3 dir) {
		position = pos;
		direction = dir;
	}

	Vector3 pointAt(float t) {
		return position + t*direction;
	}

	Vector3 getDirection() { return direction; }
	Vector3 getPosition() { return position; }

};

