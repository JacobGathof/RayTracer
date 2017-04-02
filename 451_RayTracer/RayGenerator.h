#pragma once
#include "Camera.h"
#include "Ray.h"
#include <cmath>

class RayGenerator {

private:
	Camera *camera;
	int RESX, RESY;

public:
	RayGenerator(Camera *camera, int RESX, int RESY) {
		this->camera = camera;
		this->RESX = RESX;
		this->RESY = RESY;
	}

	Ray getRay(int x, int y) {
		Vector3 s;
		y = RESY - y;
		float u = -RESX / 2.0f + (RESX)*(x + .5f) / (RESX);
		float v = -RESY / 2.0f + (RESY)*(y + .5f) / (RESY);
		float d = 1 / tan(camera->FOV / 2) * RESX / 2;

		s = camera->position + camera->u*u + camera->v*v - camera->w*d;

		Vector3 rayDir = (s - camera->position).normalize();

		return Ray(camera->position, rayDir);
	}

};