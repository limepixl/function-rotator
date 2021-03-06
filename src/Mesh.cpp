#include "mesh.h"
#include <glad/glad.h>

Mesh::Mesh(const std::vector<float>& vertexPositions, const std::vector<unsigned int>& indices, const std::vector<float>& normals)
{	
	vertexCount = static_cast<int>(indices.size());

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	
	// Passing vertex positions
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertexPositions.size() * sizeof(float), vertexPositions.data(), GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);
	glEnableVertexAttribArray(0);

	// Passing normals
	unsigned int nVBO;
	glGenBuffers(1, &nVBO);
	glBindBuffer(GL_ARRAY_BUFFER, nVBO);
	glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(float), normals.data(), GL_STATIC_DRAW);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);
	glEnableVertexAttribArray(1);
	
	// Creating EBO for indexed rendering using indices
	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned	int), indices.data(), GL_STATIC_DRAW);
	
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

Mesh::Mesh(const std::vector<float>& vertexPositions)
{
	vertexCount = static_cast<int>(vertexPositions.size()) / 3;

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	
	// Passing vertex positions
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertexPositions.size() * sizeof(float), vertexPositions.data(), GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);
	glEnableVertexAttribArray(0);
	
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void Mesh::Draw()
{
	// Bind
	glBindVertexArray(VAO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

	glDrawElements(GL_TRIANGLES, vertexCount, GL_UNSIGNED_INT, nullptr);

	// Unbind
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindVertexArray(0);	
}

void Mesh::DrawNonIndexed()
{
	// Bind
	glBindVertexArray(VAO);

	glDrawArrays(GL_LINE_STRIP, 0, vertexCount);

	// Unbind
	glBindVertexArray(0);
}