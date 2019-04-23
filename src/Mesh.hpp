#pragma once
#include <vector>

struct Mesh
{
	unsigned int VAO;
	
	Mesh() = default;
	~Mesh();
	Mesh(std::vector<float>& vertexPositions);
	
	void bind();
	void unbind();
};

