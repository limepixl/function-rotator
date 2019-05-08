#include "Mesh.hpp"
#include <glad/glad.h>

Mesh::Mesh(const std::vector<float>& vertexPositions, const std::vector<unsigned int>& indices, const std::vector<float>& normals)
{
	// Check if indices have been given
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

	// Check if normals have been given
	if(!normals.empty())
		hasNormals = true;
	else
		hasNormals = false;
	
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	
	// Passing vertex positions
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertexPositions.size() * sizeof(float), vertexPositions.data(), GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);
	glEnableVertexAttribArray(0);

	if(hasNormals)
	{
		// Passing normals
		unsigned int nVBO;
		glGenBuffers(1, &nVBO);
		glBindBuffer(GL_ARRAY_BUFFER, nVBO);
		glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(float), normals.data(), GL_STATIC_DRAW);

		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);
		glEnableVertexAttribArray(1);
	}
	
	if(hasIndices)
	{
		// Creating EBO for indexed rendering using indices
		glGenBuffers(1, &EBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned	int), indices.data(), GL_STATIC_DRAW);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}
	
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

Mesh::~Mesh()
{
	glDeleteBuffers(1, &EBO);
	glDeleteBuffers(1, &VBO);
	glDeleteVertexArrays(1, &VAO);
}

void Mesh::draw()
{
	if(hasIndices)
	{
		// Bind
		glBindVertexArray(VAO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

		glDrawElements(GL_TRIANGLES, vertexCount, GL_UNSIGNED_INT, nullptr);

		// Unbind
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
	} else
	{
		// Bind
		glBindVertexArray(VAO);

		glDrawArrays(GL_LINE_STRIP, 0, vertexCount);

		// Unbind
		glBindVertexArray(0);
	}
}