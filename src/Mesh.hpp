#pragma once
#include <vector>

struct Mesh
{
	unsigned int VAO;
	unsigned int EBO;
	int vertexCount;
	bool hasIndices;

	Mesh() = default;
	~Mesh();
	Mesh(const std::vector<float>& vertexPositions, const std::vector<unsigned int>& indices = std::vector<unsigned int>());

	void bind();
	void unbind();
};
