#pragma once
#include "Primitive.h"

class Sphere : public Primitive {

public:

	float radius;
	
	virtual HitPoint intersectRay(Ray ray) {
	

		float a = ray.getDirection().dot(ray.getDirection());
		float b = 2.0f*ray.getDirection().dot((ray.getPosition() - position));
		float c = (ray.getPosition() - position).dot((ray.getPosition() - position)) - radius*radius;

		float d = b*b - 4 * a*c;
		if (d < 0) return HitPoint(this, 0, false);

		float t0 = (-b + sqrt(d)) /(2*a);
		float t1 = (-b - sqrt(d)) /(2*a);

		if (t0 < 0 && t1 < 0) { t0 = 0; t1 = 0; }

		return HitPoint(this, t0 < t1 ? t0 : t1, true);
	
	};


	virtual Color colorAt(Vector3 position) { 
		return Color(255, 255, 255); 
	}

	virtual Vector3 getNormalAt(Vector3 pos) {
		Vector3 normal = (pos - position).normalize();
		return normal;
	}


	Sphere(Vector3 position, float radius, int mat) {
		this->position = position;
		this->radius = radius;
		this->material_ID = mat;

		this->min = position - Vector3(radius, radius, radius);
		this->max = position + Vector3(radius, radius, radius);
	}

};