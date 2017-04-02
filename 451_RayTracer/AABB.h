#pragma once
#include "Primitive.h"
#include <algorithm>

class AABB : public Primitive {

public:

	Vector3 currentNormal = Vector3(0,0,0);
	Vector3 bbMin;
	Vector3 bbMax;

	AABB() {
		bbMin = Vector3(INFINITY, INFINITY, INFINITY);
		bbMax = Vector3(-INFINITY, -INFINITY, -INFINITY);
	}

	AABB(Vector3 min, Vector3 max) {
		bbMin = min;
		bbMax = max;
	}

	void addPrimitive(Primitive* p) {

		if (p->min[0] <= bbMin[0]) bbMin[0] = p->min[0];
		if (p->max[0] >= bbMax[0]) bbMax[0] = p->max[0];

		if (p->min[1] <= bbMin[1]) bbMin[1] = p->min[1];
		if (p->max[1] >= bbMax[1]) bbMax[1] = p->max[1];

		if (p->min[2] <= bbMin[2]) bbMin[2] = p->min[2];
		if (p->max[2] >= bbMax[2]) bbMax[2] = p->max[2];

	}

	bool checkPrimitive(Primitive* p) {
		//std::cout << p->position << "***" << bbMin << "***" << bbMax << std::endl;
		return ((p->position[0] >= bbMin[0]) && (p->position[0] <= bbMax[0]) && (p->position[1] >= bbMin[1]) && (p->position[1] <= bbMax[1]) && (p->position[2] >= bbMin[2]) && (p->position[2] <= bbMax[2]));
	}

	virtual HitPoint intersectRay(Ray ray){

		HitPoint hp = HitPoint(this, -1, false);

		float entrance = 0.0f;
		float exit = INFINITY;
		Vector3 normal = Vector3(0, 0, 0);

		for (int i = 0; i<3; i++)
		{
			float slabA = bbMin[i];
			float slabB = bbMax[i];
			float invDir = 1.0f / ray.getDirection()[i];
			float origin = ray.getPosition()[i];

			float closestHit = (slabA - origin) * invDir;
			float farthestHit = (slabB - origin) * invDir;

			if (farthestHit < closestHit)
				std::swap(closestHit, farthestHit);

			bool tooClose = farthestHit < entrance;
			bool tooFar = closestHit > exit;


			if (tooClose || tooFar)
				return HitPoint(0,-1,false);


			bool foundNewEntrance = closestHit > entrance;
			entrance = foundNewEntrance ? closestHit : entrance;

			bool foundNewExit = farthestHit < exit;
			exit = foundNewExit ? farthestHit : exit;

			if (foundNewEntrance)
			{
				normal = Vector3(0, 0, 0);
				normal[i] = ray.getDirection()[i] * -1;
				normal.normalize();
			}
		}

		hp.object = this;
		currentNormal = normal;
		hp.t = entrance;
		hp.hit = true;

		return hp;
	}

	virtual Vector3 getNormalAt(Vector3 pos) {
		return currentNormal;
	}







};
