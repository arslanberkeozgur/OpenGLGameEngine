#include "Mesh.h"
#include <stdio.h>

Mesh::Mesh(Geometry geo)
{
	VAO = 0;
	VBO = 0;
	IBO = 0;
	indexCount = 0;
	vertices = nullptr;
	indices = nullptr;
	numOfIndices = 0;
	numOfVertices = 0;
	geometry = geo;

	if (geometry == CUBE)
	{
		// Create a cube
		GLfloat vrt[] = {
			// Front face
			-1.0f, -1.0f,  1.0f,
			1.0f, -1.0f,  1.0f,
			1.0f,  1.0f,  1.0f,
			-1.0f,  1.0f,  1.0f,
			// Back face
			-1.0f, -1.0f, -1.0f,
			-1.0f,  1.0f, -1.0f,
			1.0f,  1.0f, -1.0f,
			1.0f, -1.0f, -1.0f,
		};
		unsigned int ind[] = 
		{
			0, 1, 2, // Front 
			0, 2, 3,
			3, 5, 2, // Top
			5, 2, 6,
			1, 2, 7, // Right
			2, 6, 7,
			0, 3, 5, // Left 
			0, 4, 5,
			4, 7, 5, // Back
			7, 6, 5,
			0, 4, 1, // Bottom 
			1, 4, 7
		};
		SetVertices(vrt, sizeof(vrt) / sizeof(GLfloat));
		SetIndices(ind, sizeof(ind) / sizeof(unsigned int));
	}
	else if (geometry == PYRAMID)
	{
		// Create a pyramid
		GLfloat vrt[] = {
			-1.0, -1.0, 0.0,
			1.0, -1.0, 0.0,
			0.0, 1.0, 0.0,
			0.0, 0.0, -1.0
		};
		unsigned int ind[] = {
			0, 1, 2,
			0, 2, 3,
			0, 3, 1,
			1, 3, 2
		};
		SetVertices(vrt, sizeof(vrt) / sizeof(GLfloat));
		SetIndices(ind, sizeof(ind) / sizeof(unsigned int));
	}
}

void Mesh::SetVertices(GLfloat* v, unsigned int num)
{
	vertices = new GLfloat[num];
	for (int i = 0; i < num; i++)
	{
		vertices[i] = v[i];
	}
	this->numOfVertices = num;
}

void Mesh::SetIndices(unsigned int* ind, unsigned int num)
{
	indices = new unsigned int[num];
	for (int i = 0; i < num; i++)
	{
		indices[i] = ind[i];
	}
	numOfIndices = num;
}

void Mesh::CreateMesh()
{
	indexCount = numOfIndices;
	
	// Generate a VAO ID.
	glGenVertexArrays(1, &VAO);
	// Bind the VAO ID.
	glBindVertexArray(VAO);

		// Generate an IBO ID.
		glGenBuffers(1, &IBO);
		// Bind the IBO ID.
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
		// Copy the index data to the IBO.
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices[0]) * numOfIndices, indices, GL_STATIC_DRAW);

		// Generate a VBO ID.
		glGenBuffers(1, &VBO);
		// Bind the VBO ID.
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		// Copy the vertex data to the VBO.
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices[0]) * numOfVertices, vertices, GL_STATIC_DRAW);

			// Enable the vertex attribute array for position.
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (GLvoid*)0);

			// Enable the vertex attribute array for color.
			glEnableVertexAttribArray(0);

		// Unbind the VBO ID.
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	// Unbind the VAO ID.
	glBindVertexArray(0);
	// Unbind IBO ID 
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

// Draws the mesh, then unbinds all buffers, clearing them for future renders.
void Mesh::RenderMesh()
{
	glBindVertexArray(VAO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
			glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void Mesh::ClearMesh()
{
	if (IBO != 0)
	{
		glDeleteBuffers(1, &IBO);
		IBO = 0;
	}
	if (VBO != 0)
	{
		glDeleteBuffers(1, &VBO);
		VBO = 0;
	}
	if (VAO != 0)
	{
		glDeleteVertexArrays(1, &VAO);
		VAO = 0;
	}

	indexCount = 0;
}

Mesh::~Mesh()
{
	//printf("Destroyed mesh \n");
	ClearMesh();
}