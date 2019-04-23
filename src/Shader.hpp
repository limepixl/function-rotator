#pragma once
#include <glm/glm.hpp>

struct Shader
{
    unsigned int ID;
    
    Shader(const char* vertexPath, const char* fragmentPath);
    
    void useProgram();
    void stopUsingProgram();
	
	void setInt(int value, const char* location);
	void setFloat(float value, const char* location);
	void setMat4(const glm::mat4& value, const char* location);
};
