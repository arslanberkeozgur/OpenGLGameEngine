#pragma once

#include <GL\glew.h>

enum Geometry
{
	CUBE,
	PYRAMID
};

class Mesh
{
public:
	Mesh(Geometry geometry);
	~Mesh();

	void CreateMesh();
	void RenderMesh();
	void ClearMesh();

	void SetVertices(GLfloat* v, unsigned int num);
	void SetIndices(unsigned int* ind, unsigned int num);

	GLfloat* GetVertices() { return vertices; };

private:
	GLuint VAO, VBO, IBO;
	GLsizei indexCount;
	GLfloat* vertices;
	unsigned int* indices;
	unsigned int numOfVertices;
	unsigned int numOfIndices;
	Geometry geometry;
};

