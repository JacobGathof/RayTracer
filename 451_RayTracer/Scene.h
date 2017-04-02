#pragma once
#include "objLoader.hpp"
#include <vector>
#include "Primitive.h"
#include "Sphere.h"
#include "Triangle.h"
#include "Light.h"
#include "Material.h"
#include "AABB.h"
#include "BVHTree.h"
//#include "PrimitiveArray.h"

class Scene {

public:

	BVHTree tree;
	//PrimitiveArray arr;
	std::vector<Light> lights;
	std::vector<Material> materials;
	

	void build(objLoader &data) {

		tree.buildTree(data);
		processMaterials(data);
		processLights(data);
		
	}

	void processMaterials(objLoader &data) {

		for (int i = 0; i < data.materialCount; i++) {

			auto ka_ = data.materialList[i]->amb;
			auto kd_ = data.materialList[i]->diff;
			auto ks_ = data.materialList[i]->spec;
			float re = (float)data.materialList[i]->reflect;

			Vector3 ka = Vector3(ka_[0], ka_[1], ka_[2]);
			Vector3 kd = Vector3(kd_[0], kd_[1], kd_[2]);
			Vector3 ks = Vector3(ks_[0], ks_[1], ks_[2]);

			Material mat = Material(ka, kd, ks, re);
			materials.push_back(mat);
		}
	}

	void processLights(objLoader &data) {

		for (int i = 0; i < data.lightPointCount; i++) {

			int index = data.lightPointList[i]->pos_index;
			int mat_index = data.lightPointList[i]->material_index;

			Vector3 pos = Vector3(data.vertexList[index]->e[0], data.vertexList[index]->e[1], data.vertexList[index]->e[2]);

			auto ka_ = data.materialList[mat_index]->amb;
			auto kd_ = data.materialList[mat_index]->diff;
			auto ks_ = data.materialList[mat_index]->spec;

			Vector3 ka = Vector3(ka_[0], ka_[1], ka_[2]);
			Vector3 kd = Vector3(kd_[0], kd_[1], kd_[2]);
			Vector3 ks = Vector3(ks_[0], ks_[1], ks_[2]);

			Light light = Light(pos, ka, kd, ks);
			lights.push_back(light);
		}
	}

	HitPoint intersect(Ray ray) {

		HitPoint p = tree.intersect(ray);

		/*
		HitPoint best = HitPoint(0, -1, false);
		for (auto p : tree.primitives) {

			HitPoint hp = p->intersectRay(ray);

			if (hp.hit && (best.object == 0 || (hp.t < best.t))) {
				if (hp.t < .1 || hp.object == 0) continue;
				best = hp;
			}
		}

		return best;
		*/

		return p;
	}


	void clean() {
		tree.clean();
		
	}


	Scene() {}
	~Scene() { 
		clean(); 
	}

};
