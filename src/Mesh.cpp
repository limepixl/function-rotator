#include "Mesh.hpp"
#include <GL/glew.h>

Mesh::~Mesh()
{
	glDeleteVertexArrays(1, &VAO);
}

Mesh::Mesh(std::vector<float>& vertexPositions)
{
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	
	// Passing vertex positions
	unsigned int VBO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertexPositions.size() * sizeof(float), vertexPositions.data(), GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);
	glEnableVertexAttribArray(0);
	
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void Mesh::bind()
{
	glBindVertexArray(VAO);
}

void Mesh::unbind()
{
	glBindVertexArray(0);
}
