#pragma once
#include <glm/glm.hpp>

struct Shader
{
	unsigned int ID;

	Shader(const char* vertexPath, const char* fragmentPath);

	void UseProgram();
	void StopUsingProgram();

	void SetInt(int value, const char* location);
	void SetFloat(float value, const char* location);
	void SetMat4(const glm::mat4& value, const char* location);
	void SetVec3(float x, float y, float z, const char* location);
};
