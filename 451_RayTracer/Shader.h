#pragma once
#include "Ray.h"
#include "HitPoint.h"
#include <algorithm>


class Shader {


public:

	Scene *scene;
	Camera* camera;

	void build(Scene *scene, Camera* camera) {
		this->scene = scene;
		this->camera = camera;
	}


	Vector3 shade(HitPoint hp, Ray ray, float currentReflectiveParameter) {

		if (!hp.hit) {	return Vector3(0, 0, 0);}


		Vector3 hitPos = ray.pointAt(hp.t);
		Material material = scene->materials[hp.object->material_ID];


		Vector3 totalColor = Vector3(0, 0, 0);
		bool isReflective = material.r > 0.0f;

		if (isReflective && currentReflectiveParameter > .05f) {
			Ray r = Ray(hitPos, reflect(ray.getDirection(), hp.object->getNormalAt(hitPos)));
			HitPoint newHitPoint = scene->intersect(r);

			totalColor += currentReflectiveParameter*shade(newHitPoint, r, currentReflectiveParameter*material.r);
		}


		for (auto l : scene->lights) {

			Vector3 ambient = calculateAmbient(material, l);
			Vector3 diffuse = calculateDiffuse(material, l, hp, ray);
			Vector3 specular = calculateSpecular(material, l, hp, ray);
			float attenuation = calculateAttenuation(l, hp, ray);

			Vector3 shadowRayDirection = (l.position - hitPos).normalize();
			Ray shadowRay = Ray(hitPos, shadowRayDirection);
			HitPoint shadowHitPoint = scene->intersect(shadowRay);

			bool isPointInShadow = shadowHitPoint.hit && shadowHitPoint.t < l.position.distance(hitPos);
			isPointInShadow = false;

			if (isPointInShadow) {
				totalColor += (ambient+diffuse+specular)/3*attenuation*.5f;
			}
			else {
				totalColor += attenuation*(ambient + diffuse + 4.0f*specular) / 3;
			}
		}

		return totalColor;
	}



	Vector3 shadeByNormal(HitPoint hp, Vector3 hitPos) {
		return hp.object->getNormalAt(hitPos);
	}


	Vector3 calculateAmbient(Material mat, Light l) {
		return mat.ka * l.ka;
	}

	Vector3 calculateDiffuse(Material mat, Light l, HitPoint hp, Ray ray) {
		Vector3 hitPos = ray.pointAt(hp.t);
		Vector3 norm = hp.object->getNormalAt(hitPos);
		Vector3 lightDir = (l.position - hitPos).normalize();

		float d = std::max(0.0f, lightDir.dot(norm));
		
		return mat.kd * l.kd * d;
	}

	Vector3 calculateSpecular(Material mat, Light l, HitPoint hp, Ray ray) {
		Vector3 hitPos = ray.pointAt(hp.t);
		Vector3 norm = hp.object->getNormalAt(hitPos);
		Vector3 lightDir = (hitPos - l.position).normalize();

		Vector3 ref = reflect(-lightDir, norm);

		float d = std::max(0.0f, ref.dot((ray.getDirection())));

		return mat.ks * l.ks * pow(d, 16);
	}


	float calculateAttenuation(Light l, HitPoint hp, Ray ray) {
		float dist = l.position.distance(ray.pointAt(hp.t));

		float atten = 8.0f / (1 + .0008f*dist + .00002f*dist*dist);
		return atten;

	}


	Vector3 reflect(Vector3 direction, Vector3 normal) {
		return (-2 * (direction.dot(normal)) * normal + direction).normalize();
	}



};
