#pragma once
#include "Primitive.h"
#include "AABB.h"
#include "Ray.h"

class BVHTree {

public:

	std::vector<Primitive*> primitives;
	

	class BVHNode {

	public:

		BVHNode() {}
		~BVHNode() {
			if (right != 0) delete right;
			if (left != 0) delete left;
		}

		bool leaf = false;
		Primitive * contents;
		AABB bounds;

		HitPoint intersects(Ray ray) {
			
			if (bounds.intersectRay(ray).hit == false) {
				return HitPoint(0, -1, false);
			}

			if (this->leaf && contents != 0) {
				return contents->intersectRay(ray);
			}
			if (this->leaf && contents == 0) {
				return HitPoint(0, -1, false);
			}

			HitPoint a = HitPoint(0, -1, false);
			HitPoint b = HitPoint(0, -1, false);
			
			a = left->intersects(ray);
			b = right->intersects(ray);

			HitPoint best = HitPoint(0, -1, false);

			if (a.hit && (best.object == 0 || (a.t < best.t))) {
				if (a.t < 0.005f || a.object == 0) {}
				else { best = a; }
			}
			if (b.hit && (best.object == 0 || (b.t < best.t))) {
				if (b.t < 0.005f || b.object == 0) {}
				else { best = b; }
			}

			/*
			if (b.hit) return b;

			if (!a.hit && !b.hit ) return HitPoint(0, -1, false);
			if (!a.hit && b.hit) return b;
			if (a.hit && !b.hit) return a;


			if(a.t < .001f && b.t < .001f) return HitPoint(0, -1, false);
			if (a.t < .001f && b.t > .001f) return b;
			if (b.t < .001f && a.t > .001f) return a;

			if(a.t < b.t) return a;
			return b;
			*/
			return best;
		}

		void buildNode(std::vector<Primitive*> primitives) {

			AABB box;
			for (auto p : primitives) {
				box.addPrimitive(p);
			}
			bounds = box;

			if (primitives.size() == 1) {
				//std::cout << this->bounds.bbMin << "," << this->bounds.bbMax << std::endl;
				this->contents = primitives[0];
				this->leaf = true;
				return;
			}

			if (primitives.size() == 0) {
				this->contents = 0;
				this->leaf = true;
				return;
			}

			int maxAxis = -1;
			if (bounds.bbMax[0] - bounds.bbMin[0] >= bounds.bbMax[1] - bounds.bbMin[1]) maxAxis = 0;
			if (bounds.bbMax[1] - bounds.bbMin[1] >= bounds.bbMax[2] - bounds.bbMin[2]) maxAxis = 1;
			if (bounds.bbMax[2] - bounds.bbMin[2] >= bounds.bbMax[0] - bounds.bbMin[0]) maxAxis = 2;

			Vector3 half1 = box.bbMax - Vector3(
					(maxAxis == 0) ? (box.bbMax[0] - box.bbMin[0]) / 2 : 0,
					(maxAxis == 1) ? (box.bbMax[1] - box.bbMin[1]) / 2 : 0,
					(maxAxis == 2) ? (box.bbMax[2] - box.bbMin[2]) / 2 : 0);

			Vector3 half2 = box.bbMin + Vector3(
				(maxAxis == 0) ? (box.bbMax[0] - box.bbMin[0]) / 2 : 0,
				(maxAxis == 1) ? (box.bbMax[1] - box.bbMin[1]) / 2 : 0,
				(maxAxis == 2) ? (box.bbMax[2] - box.bbMin[2]) / 2 : 0);

			AABB leftBounds = AABB(box.bbMin, half1);
			AABB rightBounds = AABB(half2, box.bbMax);


			left = new BVHNode();
			right = new BVHNode();

			left->bounds = leftBounds;
			right->bounds = rightBounds;

			//std::cout << leftBounds.bbMin << "   " << leftBounds.bbMax << std::endl;
			//std::cout << rightBounds.bbMin << "   " << rightBounds.bbMax << std::endl;

			left->contents = 0;
			right->contents = 0;

			std::vector<Primitive *> leftVector;
			std::vector<Primitive *> rightVector;

			for (auto p : primitives) {

				if (left->bounds.checkPrimitive(p)) {
					leftVector.push_back(p);
				}
				else if (right->bounds.checkPrimitive(p)) {
					rightVector.push_back(p);
				}
				else {
					leftVector.push_back(p);
				}
			}

			//std::cout << "==" << leftVector.size() <<"," << rightVector.size() << std::endl;;

			if (leftVector.size() == 0 && rightVector.size() > 1) {
				Primitive* p = rightVector[rightVector.size() - 1];
				rightVector.pop_back();
				leftVector.push_back(p);
			}

			if (rightVector.size() == 0 && leftVector.size() > 1) {
				Primitive* p = leftVector[leftVector.size() - 1];
				leftVector.pop_back();
				rightVector.push_back(p);
			}

			left->buildNode(leftVector);
			right->buildNode(rightVector);

		}

		BVHNode *left = 0, *right = 0;
	};


	BVHNode * root;


	HitPoint intersect(Ray ray) {

		HitPoint p = root->intersects(ray);
		return p;

	}


	void buildTree(objLoader &data) {
		root = new BVHNode();

		processSpheres(data);
		processTriangles(data);
		

		root->buildNode(primitives);
		
	}

	void processTriangles(objLoader &data) {

		for (int i = 0; i < data.faceCount; i++) {
			Vector3 a, b, c, d;
			int index;

			index = data.faceList[i]->vertex_index[0];
			a = Vector3(data.vertexList[index]->e[0], data.vertexList[index]->e[1], data.vertexList[index]->e[2]);

			index = data.faceList[i]->vertex_index[1];
			b = Vector3(data.vertexList[index]->e[0], data.vertexList[index]->e[1], data.vertexList[index]->e[2]);

			index = data.faceList[i]->vertex_index[2];
			c = Vector3(data.vertexList[index]->e[0], data.vertexList[index]->e[1], data.vertexList[index]->e[2]);

			int material = data.faceList[i]->material_index;


			Triangle *t = new Triangle(a, b, c, material);
			primitives.push_back(t);

		}
	}

	void processSpheres(objLoader &data) {

		for (int i = 0; i < data.sphereCount; i++) {
			int pos_index = data.sphereList[i]->pos_index;
			int rad_index = data.sphereList[i]->up_normal_index;

			Vector3 pos = Vector3(
				data.vertexList[pos_index]->e[0],
				data.vertexList[pos_index]->e[1],
				data.vertexList[pos_index]->e[2]
				);

			Vector3 radVector = Vector3(
				data.normalList[rad_index]->e[0],
				data.normalList[rad_index]->e[1],
				data.normalList[rad_index]->e[2]
				);

			float radius = radVector.length();

			int material = data.sphereList[i]->material_index;

			Sphere *s = new Sphere(pos, radius, material);
			primitives.push_back(s);

		}
	}


	void clean() {
		for (auto a : primitives) {
			delete a;
		}
		delete root;

	}



};
