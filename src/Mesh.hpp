#pragma once
#include <vector>

struct Mesh
{
	unsigned int VAO;
	unsigned int EBO;
	int vertexCount;
	bool hasIndices;
	bool hasNormals;

	Mesh() = default;
	~Mesh();
	Mesh(const std::vector<float>& vertexPositions,
		 const std::vector<unsigned int>& indices = std::vector<unsigned int>(),
		 const std::vector<float>& normals = std::vector<float>());

	void bind();
	void unbind();
};
