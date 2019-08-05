#pragma once
#include <vector>

struct Mesh
{
	int vertexCount;

	unsigned int VAO;	// Vertex Array Object
	unsigned int VBO;	// Positions buffer
	unsigned int EBO;	// Index buffer

	Mesh() = default;

	Mesh(const std::vector<float>& vertexPositions,
		 const std::vector<unsigned int>& indices,
		 const std::vector<float>& normals);

	Mesh(const std::vector<float>& vertexPositions);

	void draw();
	void drawNonIndexed();
};
