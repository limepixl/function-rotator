#include "Mesh.hpp"
#include <GL/glew.h>

Mesh::~Mesh()
{
	glDeleteVertexArrays(1, &VAO);
}

Mesh::Mesh(const std::vector<float>& vertexPositions, const std::vector<unsigned int>& indices)
{
	if(!indices.empty())
	{
		vertexCount = static_cast<int>(indices.size());
		hasIndices = true;
	}
	else 
	{
		vertexCount = static_cast<int>(vertexPositions.size() / 3);		
		hasIndices = false;
	}
	
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	
	// Passing vertex positions
	unsigned int VBO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertexPositions.size() * sizeof(float), vertexPositions.data(), GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);
	glEnableVertexAttribArray(0);
	
	if(hasIndices)
	{
		glGenBuffers(1, &EBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned	int), indices.data(), GL_STATIC_DRAW);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}
	
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void Mesh::bind()
{
	glBindVertexArray(VAO);
	
	if(hasIndices)
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
}

void Mesh::unbind()
{
	if(hasIndices)
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	
	glBindVertexArray(0);
}
