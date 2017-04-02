#pragma once
#include "GenVector.h"
#include "Ray.h"
#include "HitPoint.h"

class Primitive {

public :
	
	Vector3 position;
	Vector3 min, max;
	int material_ID;

	virtual HitPoint intersectRay(Ray ray) {return HitPoint(this, -1, false); }
	virtual Color colorAt(Vector3 position) { return Color(0, 0, 0); }
	virtual Vector3 getNormalAt(Vector3 pos) {return Vector3(1,0,0); }

};
