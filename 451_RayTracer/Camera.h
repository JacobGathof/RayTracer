#pragma once
#include "GenVector.h"

class Camera {

public:
	Vector3 u, v, w;
	Vector3 position;
	float FOV;

	Camera() {};
	void build(objLoader &objData) {
	
		Vector3 pos = Vector3(
			objData.vertexList[objData.camera->camera_pos_index]->e[0],
			objData.vertexList[objData.camera->camera_pos_index]->e[1],
			objData.vertexList[objData.camera->camera_pos_index]->e[2]
			);

		Vector3 lookat = Vector3(
			objData.vertexList[objData.camera->camera_look_point_index]->e[0],
			objData.vertexList[objData.camera->camera_look_point_index]->e[1],
			objData.vertexList[objData.camera->camera_look_point_index]->e[2]
			);

		Vector3 up = Vector3(
			objData.normalList[objData.camera->camera_up_norm_index]->e[0],
			objData.normalList[objData.camera->camera_up_norm_index]->e[1],
			objData.normalList[objData.camera->camera_up_norm_index]->e[2]
			);


		Vector3 g = (lookat - pos).normalize();
		up = up.normalize();

		this->position = pos;
		w = -g;
		u = up.cross(w).normalize();
		v = w.cross(u).normalize();
		FOV = 90.0f * 3.14159f / 180.0f;
	}

};