#pragma once
#include <vector>

struct Mesh
{
	int vertexCount;
	bool hasIndices;
	bool hasNormals;

	unsigned int VAO;	// Vertex Array Object
	unsigned int VBO;	// Positions buffer
	unsigned int EBO;	// Index buffer

	Mesh() = default;

	Mesh(const std::vector<float>& vertexPositions,
		 const std::vector<unsigned int>& indices = std::vector<unsigned int>(),
		 const std::vector<float>& normals = std::vector<float>());

	~Mesh();

	void draw();
};
