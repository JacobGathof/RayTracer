#pragma once
#include "glew.h"
#include "glfw3.h"
#include <iostream>
#include "ShaderProgram.h"

#include "GenVector.h"
#include "simplePNG.h"
#include "Buffer.h"
#include "objLoader.hpp"
#include "Camera.h"
#include "Ray.h"
#include "RayGenerator.h"
#include "Scene.h"
#include "Shader.h"


#define RES 100


Camera camera;
bool up, down, left, right, space, shift, q, e,  m;
GLFWwindow* window;

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {


	if (action == GLFW_PRESS && key == GLFW_KEY_ESCAPE) {
		glfwSetWindowShouldClose(window, true);
		return;
	}

	if (action == GLFW_PRESS) {

		switch (key) {
		case GLFW_KEY_W: up = true; break;
		case GLFW_KEY_S: down = true; break;
		case GLFW_KEY_A: left = true; break;
		case GLFW_KEY_D: right = true; break;
		case GLFW_KEY_SPACE: space = true; break;
		case GLFW_KEY_LEFT_SHIFT: shift = true; break;
		case GLFW_KEY_Q: q = true; break;
		case GLFW_KEY_E: e = true; break;

		case GLFW_KEY_M: m = true; break;
		}


	}

	if (action == GLFW_RELEASE) {
		switch (key) {
		case GLFW_KEY_W: up = false; break;
		case GLFW_KEY_S: down = false; break;
		case GLFW_KEY_A: left = false; break;
		case GLFW_KEY_D: right = false; break;
		case GLFW_KEY_SPACE: space = false; break;
		case GLFW_KEY_LEFT_SHIFT: shift = false; break;
		case GLFW_KEY_Q: q = false; break;
		case GLFW_KEY_E: e = false; break;

		case GLFW_KEY_M: m = false; break;
		}
	}
}

float speed = 20.0f;

void render(float* color_data, GLuint VBO_color) {
	float dt = .1f;
	

	glBindBuffer(GL_ARRAY_BUFFER, VBO_color);
	glBufferSubData(GL_ARRAY_BUFFER, 0, RES*RES * 3 * 4, color_data);
	glVertexAttribPointer(1, 3, GL_FLOAT, false, 0, 0);

	glDrawArrays(GL_POINTS, 0, RES*RES);


	glfwPollEvents();
	glfwSwapBuffers(window);

	if (up) camera.position = camera.position + -dt*speed*camera.w;
	if (down) camera.position = camera.position + dt*speed*camera.w;
	if (left) camera.position = camera.position + -dt*speed*camera.u;
	if (right) camera.position = camera.position + dt*speed*camera.u;
	if (space) camera.position = camera.position + dt*speed*camera.v;
	if (shift) camera.position = camera.position + -dt*speed*camera.v;

	if (q) {
		Vector3 u = camera.u;
		Vector3 newU = Vector3(u[0] * cos(dt) + u[2] * sin(dt), u[1], -u[0] * sin(dt) + u[2] * cos(dt));
		camera.u = newU;

		Vector3 w = camera.w;
		Vector3 newW = Vector3(w[0] * cos(dt) + w[2] * sin(dt), w[1], -w[0] * sin(dt) + w[2] * cos(dt));
		camera.w = newW;

	}

	if (e) {
		Vector3 u = camera.u;
		Vector3 newU = Vector3(u[0] * cos(-dt) + u[2] * sin(-dt), u[1], -u[0] * sin(-dt) + u[2] * cos(-dt));
		camera.u = newU;

		Vector3 w = camera.w;
		Vector3 newW = Vector3(w[0] * cos(-dt) + w[2] * sin(-dt), w[1], -w[0] * sin(-dt) + w[2] * cos(-dt));
		camera.w = newW;

	}


	
}




int main() {


	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, 0);

	window = glfwCreateWindow(800, 800, "Ray Tracer", 0, 0);
	glfwMakeContextCurrent(window);


	glfwSetKeyCallback(window, key_callback);

	glewExperimental = GL_TRUE;
	GLenum err = glewInit();
	if (err != GLEW_OK)
		std::cout << "Error:" << glewGetErrorString(err);

	glEnable(GL_BLEND);
	glClearColor(0, 0, 0, 1);



	glfwSetTime(0);
	float dt = 0;
	float last_second = 0;
	float last_tick = 0;
	float last_time = 0;
	float current_time = 0;
	int framesPerSecond = 0;

	ShaderProgram shaderprog = ShaderProgram();
	shaderprog.compileShader("vertex_shader.txt", 0, "fragment_shader.txt");
	shaderprog.use();




	Buffer<Color> buffer = Buffer<Color>(RES, RES);
	Buffer<Vector3> temp_buffer = Buffer<Vector3>(RES, RES);



	objLoader objData = objLoader();
	objData.load("cornell_box.txt");


	Scene scene;
	scene.build(objData);

	
	camera.build(objData);

	Shader shader;
	shader.build(&scene, &camera);

	glEnable(GL_VERTEX_PROGRAM_POINT_SIZE);

	RayGenerator generator = RayGenerator(&camera, RES, RES);

	GLuint VAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);


	float * color_data_temp = new float[3];
	float * color_data = new float[RES*RES * 3];
	float * pos_data = new float[RES*RES * 2];

	int counter = 0;
	for (int y = 0; y < RES; y++) {
		for (int x = 0; x < RES; x++) {

			pos_data[2 * counter + 0] = 1.0f*x / RES;
			pos_data[2 * counter + 1] = 1.0f*y / RES;

			counter++;

		}
	}


	GLuint VBO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, RES*RES*2*4, pos_data, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 2, GL_FLOAT, false, 0, 0);
	glEnableVertexAttribArray(0);

	GLuint VBO_color;
	glGenBuffers(1, &VBO_color);
	glBindBuffer(GL_ARRAY_BUFFER, VBO_color);
	glBufferData(GL_ARRAY_BUFFER, RES*RES * 3 * 4, color_data, GL_STATIC_DRAW);
	glVertexAttribPointer(1,3, GL_FLOAT, false, 0, 0);
	glEnableVertexAttribArray(1);


	float gameTime = 0;
	float max = 1.0f;

	while (!glfwWindowShouldClose(window)) {
		
		for (int i = 0; i < RES; i++) {
			for (int j = 0; j < RES; j++) {

				Ray r = generator.getRay(i, j);
				Vector3 c = Vector3(0, 0, 0);

				HitPoint p = scene.intersect(r);

				c = shader.shade(p, r, 1);

				if (c[0] > max) max = c[0];
				if (c[1] > max) max = c[1];
				if (c[2] > max) max = c[2];

				buffer.at(i,j) = c;

				int counter = i + j*RES;

				Vector3 vec = Vector3(buffer.at(i, j)[0] / max, buffer.at(i, j)[1] / max, buffer.at(i, j)[2] / max);

				color_data[3 * counter + 0] = vec[0];
				color_data[3 * counter + 1] = vec[1];
				color_data[3 * counter + 2] = vec[2];
			}
		}

		render(color_data, VBO_color);
	}


	delete[] color_data;
	delete[] pos_data;

	glfwTerminate();


	return 0;
}



