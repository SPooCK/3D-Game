#pragma once
#ifndef BOX_O
#define BOX_O
// GLFW
#include <GLFW/glfw3.h>
// GLM
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>

using namespace glm;
GLfloat RandomFloat(GLfloat a, GLfloat b) {
	GLfloat random = ((GLfloat)rand()) / (GLfloat)RAND_MAX;
	GLfloat diff = b - a;
	GLfloat r = random * diff;
	return a + r;
}

// Just experimental Class for box
class Box {
public:
	vec3 pos = vec3(0, 0, 0);
	GLfloat angle = 0;
	GLfloat End = glfwGetTime() + 2.0f;
	bool pCatch = false;

	void Jump();
	bool CanJump();
};


void Box::Jump() {
	GLfloat x = pos.x; GLfloat y = pos.y; GLfloat z = pos.z;
	GLfloat R = pCatch ? 0.5f : 1.5f; GLfloat xR = RandomFloat(x - R, x + R); 
	GLfloat yR = RandomFloat(y - R, y + R); GLfloat zR = RandomFloat(z - R, z + R);
	pos = vec3(xR, yR, zR);

	End = pCatch ? glfwGetTime() + 2.0f : glfwGetTime() + 1.0f;
}

bool Box::CanJump() {
	return (glfwGetTime()>= End);
}
#endif