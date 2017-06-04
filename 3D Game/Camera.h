#pragma once

#include <vector>

#define GLEW_STATIC
#include <GL/glew.h>

#include <glm.hpp>
#include <gtc/matrix_transform.hpp>

using namespace glm;

enum Camera_Movement {
	FORWARD,
	BACKWARD,
	LEFT,
	RIGHT
};

const GLfloat YAW = -90.f;
const GLfloat PITCH = 0.0f;
const GLfloat SPEED = 6.0f;
const GLfloat SENSITIVTY = 0.25f;
const GLfloat ZOOM = 45.0f;

class Camera {
public:
	// Constructor with vectors
	Camera(vec3 position = vec3(0.0f, 0.0f, 0.0f), vec3 up = vec3(0.0f, 1.0f, 0.0f),
	GLfloat yaw = YAW, GLfloat pitch = PITCH) : front(vec3(0.0f, 0.0f, -1.0f)),
	movementSpeed(SPEED), mouseSensitivity(SENSITIVTY), zoom(ZOOM) 
	{
		this->position = position;
		this->worldUp = up;
		this->yaw = yaw;
		this->updateCameraVectors();
	}

	// Constructor with scalar values
	Camera(GLfloat posX, GLfloat posY, GLfloat posZ, GLfloat upX, GLfloat upY, GLfloat upZ, GLfloat yaw,
	GLfloat pitch) : front(vec3(0.0f, 0.0f, -1.0f)), movementSpeed(SPEED), mouseSensitivity(SENSITIVTY), zoom(ZOOM) 
	{
		this->position = vec3(posX, posY, posZ);
		this->worldUp = vec3(upX, upY, upZ);
		this->yaw = yaw;
		this->pitch = pitch;
		this->updateCameraVectors();
	}

	// Returns the view matrix calculated using Eular Angles and the LookAt Matrix
	mat4 GetViewMatrix() {
		return lookAt(this->position, this->position + this->front, this->up);
	}

	// Processes input received from any keyboard-like input system
	void ProcessKeyboard(Camera_Movement direction, GLfloat deltaTime) {
		GLfloat velocity = this->movementSpeed * deltaTime;

		if (FORWARD == direction) {
			this->position += this->front * velocity;
		}

		if (BACKWARD == direction) {
			this->position -= this->front * velocity;
		}

		if (LEFT == direction) {
			this->position -= this->right * velocity;
		}

		if (RIGHT == direction) {
			this->position += this->right * velocity;
		}
	}

	// Processes input received from a mouse input system
	void ProcessMouseMovement(GLfloat xOffset, GLfloat yOffset, GLboolean constrainPitch = true) {
		xOffset *= this->mouseSensitivity;
		yOffset *= this->mouseSensitivity;

		this->yaw += xOffset;
		this->pitch += yOffset;

		if (constrainPitch) {
			if (this->pitch > 89.0f) {
				this->pitch = 89.0f;
			}

			if (this->pitch < -89.0f) {
				this->pitch = -89.0f;
			}
		}

		this->updateCameraVectors();
	}

	// Processes input received from a mouse scroll-wheel event
	void ProcessMouseScroll(GLfloat yOffset) {
		if (this->zoom >= 1.0f && this->zoom <= 45.0f) {
			this->zoom -= yOffset;
		}

		if (this->zoom <= 1.0f) {
			this->zoom = 1.0f;
		}

		if (this->zoom >= 45.0f) {
			this->zoom = 45.0f;
		}
	}

	GLfloat GetZoom() {
		return this->zoom;
	}
};