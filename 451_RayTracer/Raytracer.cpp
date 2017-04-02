#define RES 1600


#define _USE_MATH_DEFINES //This enables math constants in Windows
#include <math.h> //Math functions and some constants

#include "GenVector.h"
#include "simplePNG.h"
#include "Buffer.h"
#include "objLoader.hpp"
#include "Camera.h"
#include "Ray.h"
#include "RayGenerator.h"
#include "Scene.h"
#include "Shader.h"

/*
int main(int argc, char ** argv)
{

	Buffer<Color> buffer = Buffer<Color>(RES, RES);
	Buffer<Vector3> temp_buffer = Buffer<Vector3>(RES, RES);

	if (argc < 3){
		printf("Usage %s input.obj output.png\n", argv[0]);
		exit(0);
	}
	

	objLoader objData = objLoader();
	objData.load(argv[1]);


	Scene scene;
	scene.build(objData);

	Camera camera;
	camera.build(objData);

	Shader shader;
	shader.build(&scene, &camera);


	RayGenerator generator = RayGenerator(camera, RES, RES);

	float max = 1.0f;

	for (int y = 0; y<RES; y++){
		for (int x = 0; x<RES; x++){


			Ray r = generator.getRay(x, y);
			Vector3 c = Vector3(0,0,0);
	
			HitPoint p = scene.intersect(r);
	
			c = shader.shade(p, r, 1);

			if (c[0] > max) max = c[0];
			if (c[1] > max) max = c[1];
			if (c[2] > max) max = c[2];

			temp_buffer.at(x, y) = c;
		}
	}


	for (int y = 0; y < RES; y++) {
		for (int x = 0; x < RES; x++) {

			Vector3 vec = temp_buffer.at(x, y) / max;
			buffer.at(x, y) = Color(vec[0]*255, vec[1]*255, vec[2]*255);
		}
	}

	simplePNG_write(argv[2], buffer.getWidth(), buffer.getHeight(), (unsigned char*)&buffer.at(0, 0));

	return 0;
}
*/