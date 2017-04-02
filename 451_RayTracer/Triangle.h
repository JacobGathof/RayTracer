#pragma once
#include "Primitive.h"
#include <iostream>
#include <algorithm>

class Triangle : public Primitive {

public:
	Vector3 v0, v1, v2, normal;

	Triangle(Vector3 v0, Vector3 v1, Vector3 v2, int mat) {
		this->v0 = v0;
		this->v1 = v1;
		this->v2 = v2;
		this->normal = ((v1 - v0).normalize().cross((v2 - v0).normalize())).normalize();
		this->material_ID = mat;
		this->position = v0;
		//std::cout << normal[0] << "," << normal[1] << "," << normal[2] << std::endl;
		this->min = Vector3(std::min(std::min(v0[0], v1[0]), v2[0]), std::min(std::min(v0[1], v1[1]), v2[1]), std::min(std::min(v0[2], v1[2]), v2[2]));
		this->max = Vector3(std::max(std::max(v0[0], v1[0]), v2[0]), std::max(std::max(v0[1], v1[1]), v2[1]), std::max(std::max(v0[2], v1[2]), v2[2]));
	}

	Triangle(Vector3 v0, Vector3 v1, Vector3 v2, Vector3 normal, int mat) {
		this->v0 = v0;
		this->v1 = v1;
		this->v2 = v2;
		this->normal = normal;
		this->material_ID = mat;
	}

	virtual HitPoint intersectRay(Ray ray) {

		float t = ((v1 - ray.getPosition()).dot(normal)) / (ray.getDirection().dot(normal));

		if (t < 0) return HitPoint(this, -1, false);

		Vector3 x = ray.pointAt(t);
		
		bool a = ((v1 - v0).cross((x - v0))).dot(normal) > 0;
		bool b = ((v2 - v1).cross((x - v1))).dot(normal) > 0;
		bool c = ((v0 - v2).cross((x - v2))).dot(normal) > 0;

		return HitPoint(this, t, (a && b && c));
		
	};

	virtual Color colorAt(Vector3 position) { 
		return Color(255, 255, 255); 
	}

	virtual Vector3 getNormalAt(Vector3 pos) {
		return normal;
	}

};