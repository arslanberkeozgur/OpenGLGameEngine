#include "Camera.h"

Camera::Camera(glm::vec3 initialPos, glm::vec3 initialUp, GLfloat initialYaw, GLfloat initialPitch, bool active)
{
	this->position = initialPos;
	this->worldUp = initialUp;
	this->yaw = initialYaw;
	this->pitch = initialPitch;
	this->isActive = active;
	this->updateCameraVectors();
}

void Camera::updateCameraVectors()
{
	front.x = cos(glm::radians(this->yaw)) * cos(glm::radians(this->pitch));
	front.y = sin(glm::radians(this->pitch));
	front.z = sin(glm::radians(this->yaw)) * cos(glm::radians(this->pitch));
	this->front = glm::normalize(front);
	this->right = glm::normalize(glm::cross(this->front, this->worldUp));
	this->up = glm::normalize(glm::cross(this->right, this->front));
}

void Camera::Follow(GLObject* obj, GLfloat radius, GLfloat height, GLfloat angle)
{
	// Setting height is the easisest.
	position.y = obj->GetPosition().y + height;

	// Put camera on the shell of the cylinder.
	position.x = obj->GetPosition().x + radius * cosf(angle);
	position.z = obj->GetPosition().z + radius * sinf(angle);

	// Make the camera point to the object
	this->front = obj->GetPosition() - this->position;
	this->front = glm::normalize(this->front);
}

glm::mat4 Camera::CalculateViewMatrix()
{
	return glm::lookAt(position, position + front, up);
}

ActiveCamera::ActiveCamera(glm::vec3 initialPos, glm::vec3 initialUp,
	GLfloat initialYaw, GLfloat initialPitch, bool active, GLfloat initialMoveSpeed, GLfloat initialTurnSpeed)
	:Camera(initialPos, initialUp, initialYaw, initialPitch, active)
{
	this->movementSpeed = initialMoveSpeed;
	this->baseSpeed = initialMoveSpeed;
	this->mouseSensitivity = initialTurnSpeed;
}

void ActiveCamera::keyControl(bool* keys, GLfloat deltaTime)
{
	if (GetActive())
	{
		if (keys[GLFW_KEY_W])
			this->position += this->front * this->movementSpeed * deltaTime;
		if (keys[GLFW_KEY_S])
			this->position -= this->front * this->movementSpeed * deltaTime;
		if (keys[GLFW_KEY_D])
			this->position += this->right * this->movementSpeed * deltaTime;
		if (keys[GLFW_KEY_A])
			this->position -= this->right * this->movementSpeed * deltaTime;
		if (keys[GLFW_KEY_E])
			this->position += this->up * this->movementSpeed * deltaTime;
		if (keys[GLFW_KEY_Q])
			this->position -= this->up * this->movementSpeed * deltaTime;
		if (keys[GLFW_KEY_LEFT_SHIFT])
			this->movementSpeed = 2 * baseSpeed;
		if (!keys[GLFW_KEY_LEFT_SHIFT])
			this->movementSpeed = baseSpeed;
	}
}

void ActiveCamera::MouseControl(GLfloat xChange, GLfloat yChange, GLfloat deltaTime, GLuint smoothness)
{
	if (GetActive())
	{

		if (smoothness < 1)
			smoothness = 1;

		if (deltaTime > 1.0f / 200)
			smoothness = 1;

		// Changes in x and y are actually always integers.
		if (abs(xChange) > 0 || abs(yChange) > 0)
		{
			GLfloat xStep = xChange / smoothness;
			GLfloat yStep = yChange / smoothness;

			for (GLint i = 0; i < smoothness; i++)
			{
				mouseQueue.push({ xStep, yStep });
			}
		}

		if (!mouseQueue.empty())
		{
			this->yaw += mouseQueue.front().first * this->mouseSensitivity * deltaTime;
			this->pitch += 2 * mouseQueue.front().second * this->mouseSensitivity * deltaTime;
			if (this->pitch > 89.0f)
				this->pitch = 89.0f;
			if (this->pitch < -89.0f)
				this->pitch = -89.0f;
			this->updateCameraVectors();
			mouseQueue.pop();
		}
	}
}