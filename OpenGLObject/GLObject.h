#pragma once

#include <list>
#include <memory>
#include <algorithm>

#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <GL\glew.h>

#include "Mesh.h"

class GLObject
{
public:
	GLObject(GLuint id, Geometry geometry, glm::vec3 color);
	~GLObject();
	
	glm::vec3 GetPosition() { return position; };
	void SetPosition(GLfloat x, GLfloat y, GLfloat z) { position = glm::vec3(x, y, z); };
	
	glm::vec3 GetRotation() { return rotation; };
	void SetRotation(GLfloat yaw, GLfloat pitch, GLfloat roll) { rotation = glm::vec3(yaw, pitch, roll); };

	glm::vec3 GetScale() { return scale; };
	void SetScale(GLfloat x, GLfloat y, GLfloat z) { scale = glm::vec3(x, y, z); };

	glm::vec3 GetVelocity() { return velocity; };
	void SetVelocity(GLfloat x, GLfloat y, GLfloat z) { velocity = glm::vec3(x, y, z); };

	glm::vec3 GetAngularVelocity() { return angularVelocity; };
	void SetAngularVelocity(GLfloat x, GLfloat y, GLfloat z) { angularVelocity = glm::vec3(x, y, z); };

	glm::vec3 GetColor() { return color; };

	Mesh* GetMesh() { return mesh; };
	void SetMesh(Mesh* mesh);

	glm::vec3 GetUp() { return up; };
	glm::vec3 GetRight() { return right; };
	glm::mat4 GetModelMatrix();

	void Update();
	void Render();

private:
	glm::vec3 position;
	glm::vec3 velocity;
	glm::vec3 rotation;
	glm::vec3 angularVelocity;
	glm::vec3 scale;
	
	glm::vec3 up;
	glm::vec3 right;

	glm::vec3 color;

	GLuint ID;

	Mesh* mesh;
};

