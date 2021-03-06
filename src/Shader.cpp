#include "shader.h"
#include <fstream>
#include <sstream>
#include <iostream>

#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>

Shader::Shader(const char* vertexPath, const char* fragmentPath)
{
	// Loading from files
	std::ifstream vertexRaw(vertexPath);
	std::ifstream fragmentRaw(fragmentPath);

	std::stringstream vertexStream;
	vertexStream << vertexRaw.rdbuf();
	vertexRaw.close();

	std::stringstream fragmentStream;
	fragmentStream << fragmentRaw.rdbuf();
	fragmentRaw.close();

	std::string vertexString = vertexStream.str();
	std::string fragmentString = fragmentStream.str();

	const char* vertexSource = vertexString.c_str();
	const char* fragmentSource = fragmentString.c_str();

	// Vertex shader creation
	unsigned int vertex = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex, 1, &vertexSource, nullptr);
	glCompileShader(vertex);

	// Compilation check
	int compiled;
	glGetShaderiv(vertex, GL_COMPILE_STATUS, &compiled);
	if (compiled != GL_TRUE)
	{
		int log_length = 0;
		char message[1024];
		glGetShaderInfoLog(vertex, 1024, &log_length, message);
		printf("%s\n", message);
	}

	// Fragment shader creation
	unsigned int fragment = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment, 1, &fragmentSource, nullptr);
	glCompileShader(fragment);

	// Compilation check
	glGetShaderiv(fragment, GL_COMPILE_STATUS, &compiled);
	if (compiled != GL_TRUE)
	{
		int log_length = 0;
		char message[1024];
		glGetShaderInfoLog(fragment, 1024, &log_length, message);
		printf("%s\n", message);
	}

	// Attribute locations
	glBindAttribLocation(ID, 0, "aPos");
	glBindAttribLocation(ID, 1, "aNormal");

	// Shader program creation
	ID = glCreateProgram();
	glAttachShader(ID, vertex);
	glAttachShader(ID, fragment);
	glLinkProgram(ID);

	// Clean up
	glDetachShader(ID, vertex);
	glDetachShader(ID, fragment);
	glDeleteShader(vertex);
	glDeleteShader(fragment);
}

void Shader::UseProgram()
{
	 glUseProgram(ID);
}

void Shader::StopUsingProgram()
{
	glUseProgram(0);
}

void Shader::SetInt(int value, const char* location)
{
	int loc = glGetUniformLocation(ID, location);
	glUniform1i(loc, value);
}

void Shader::SetFloat(float value, const char* location)
{
	int loc = glGetUniformLocation(ID, location);
	glUniform1f(loc, value);
}

void Shader::SetMat4(const glm::mat4& value, const char* location)
{
	int loc = glGetUniformLocation(ID, location);
	glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(value));
}

void Shader::SetVec3(float x, float y, float z, const char* location)
{
	int loc = glGetUniformLocation(ID, location);
	glUniform3f(loc, x, y, z);
}
