#include "GLObject.h"

GLObject::GLObject(GLuint id, Geometry geometry)
{
	SetPosition(0.0f, 0.0f, 0.0f);
	SetRotation(0.0f, 0.0f, 0.0f);
	SetScale(1.0f, 1.0f, 1.0f);
	SetVelocity(0.0f, 0.0f, 0.0f);
	SetAngularVelocity(0.0f, 0.0f, 0.0f);

	up = glm::vec3(0.0f, 1.0f, 0.0f);
	right = glm::vec3(1.0f, 0.0f, 0.0f);

	mesh = new Mesh(geometry);
	mesh->CreateMesh();

	ID = id;
}

void GLObject::SetMesh(Mesh* m)
{
	delete mesh;
	mesh = m;
	mesh->CreateMesh();
}

glm::mat4 GLObject::GetModelMatrix()
{
	glm::mat4 model(1.0f);
	model = glm::translate(model, position);
	model = glm::rotate(model, glm::radians(rotation.x), up);
	model = glm::rotate(model, glm::radians(rotation.y), right);
	model = glm::rotate(model, glm::radians(rotation.z), glm::cross(up, right));
	model = glm::scale(model, scale);
	return model;
	
}

void GLObject::Render()
{
	mesh->RenderMesh();
}

void GLObject::Update()
{
	position += velocity;
	rotation += angularVelocity;
}

GLObject::~GLObject()
{
	delete mesh;
	//printf("Destroyed object %d \n", ID);
}
