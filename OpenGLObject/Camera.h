#pragma once

#include <GL\glew.h>
#include <queue>
#include <utility>

#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>

#include <GLFW\glfw3.h>

#include "GLObject.h"

class Camera
{
public:
	Camera(
		glm::vec3 initialPos = glm::vec3(0.0, 0.0, 0.0),
		glm::vec3 initialUp = glm::vec3(0.0, 1.0, 0.0),
		GLfloat initialYaw = 0.0f,
		GLfloat initialPitch = 0.0f,
		bool active = false
		);
	
	bool GetActive() { return isActive; };
	void SetActive(bool active) { isActive = active; };

	// Follow the object from the shell of a cylinder of radius r, of height h, angle a.
	void Follow(GLObject* obj, GLfloat radius, GLfloat height, GLfloat angle);

	glm::mat4 CalculateViewMatrix();

protected:
	glm::vec3 position;
	glm::vec3 front;
	glm::vec3 up;
	glm::vec3 right;
	glm::vec3 worldUp;

	GLfloat yaw;
	GLfloat pitch;

	bool isActive;

	std::queue<std::pair<GLfloat, GLfloat>> mouseQueue;

	void updateCameraVectors();
};

class ActiveCamera : public Camera
{
public:
	ActiveCamera(
		glm::vec3 initialPos = glm::vec3(0.0, 0.0, 0.0),
		glm::vec3 initialUp = glm::vec3(0.0, 1.0, 0.0),
		GLfloat initialYaw = 0.0f,
		GLfloat initialPitch = 0.0f,
		bool active = true,
		GLfloat initialMoveSpeed = 5.0f,
		GLfloat initialTurnSpeed = 1.0f
	);

	void keyControl(bool* keys, GLfloat deltaTime);
	void MouseControl(GLfloat xChange, GLfloat yChange, GLfloat deltaTime, GLuint smoothness);
private:
	GLfloat movementSpeed;
	GLfloat baseSpeed;
	GLfloat mouseSensitivity;
};