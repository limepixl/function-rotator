#pragma once
#include <string>
#include <vector>
#include <map>
#include <cmath>
#include "mesh.h"
#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>

enum Operator
{
	ADD,
	SUB,
	MUL,
	DIV,
	POW,
	SQRT,
	SIN,
	COS,
	TAN,
	COT,
	LP,	// Left parenthesis
	RP	// Right parenthesis
};

std::map<std::string, Operator> OperatorMap;
std::map<Operator, int> Precedence;

// Split a string into tokens
inline std::vector<std::string> Split(std::string& origin, char delimiter)
{
	std::vector<std::string> result;
	for(size_t i = 0; i < origin.size(); i++)
	{
		if(origin.begin() + i + 1 == origin.end())
		{
			result.emplace_back(origin.begin(), origin.end());
			origin.erase(origin.begin(), origin.end());
			break;
		}

		if(origin[i] == delimiter)
		{
			result.emplace_back(origin.begin(), origin.begin() + i);
			origin.erase(origin.begin(), origin.begin() + i + 1);
			i = -1;
		}
	}

	return result;
}

// Check if the string is a number
inline bool IsNumber(const std::string& str)
{
	bool result = true;
	for(auto& s : str)
	{
		if(!isdigit(s))
		{
			result = false;
			break;
		}
	}

	return result;
}

// Map a value from one range to another
inline float Map(float oldmin, float oldmax, float newmin, float newmax, float current)
{
	return newmin + (newmax - newmin) * ((current - oldmin) / (oldmax - oldmin));
}

// Parse the function into tokens formatted in postfix notation
std::vector<std::string> ParseFunction(std::string& function)
{
	// TODO: Add support for non-spaced-out functions

	// Initialize operator map
	OperatorMap["+"] = ADD;
	OperatorMap["-"] = SUB;
	OperatorMap["*"] = MUL;
	OperatorMap["/"] = DIV;
	OperatorMap["^"] = POW;
	OperatorMap["sqrt"] = SQRT;
	OperatorMap["sin"] = SIN;
	OperatorMap["cos"] = COS;
	OperatorMap["tan"] = TAN;
	OperatorMap["cot"] = COT;
	OperatorMap["("] = LP;
	OperatorMap[")"] = RP;

	// Initialize precedence map
	Precedence[ADD] = 0;
	Precedence[SUB] = 0;
	Precedence[MUL] = 1;
	Precedence[DIV] = 1;
	Precedence[SIN] = 2;
	Precedence[COS] = 2;
	Precedence[TAN] = 2;
	Precedence[COT] = 2;
	Precedence[POW] = 3;
	Precedence[SQRT] = 3;
	Precedence[LP] = 4;
	Precedence[RP] = 4;

	std::vector<std::string> split = Split(function, ' ');
	
	std::vector<std::string> output;
	std::vector<std::string> stack;

	// Convert infix to postfix notation
	for(size_t i = 0; i < split.size(); i++)
	{
		std::string token = split[i];
		if(IsNumber(token) || token == "x" || token == "X")
		{
			output.push_back(token);
		} else if(OperatorMap[token] == LP)
		{
			stack.push_back(token);
		} else if(OperatorMap[token] == RP)
		{
			while(OperatorMap[stack.back()] != LP)
			{
				output.push_back(stack.back());
				stack.pop_back();
			}

			if(OperatorMap[stack.back()] == LP)
			{
				stack.pop_back();
			}
		} else
		{ 
			Operator current = OperatorMap[token];
			if(!stack.empty())
			{
				Operator top = OperatorMap[stack.back()];

				// Get the precedence of the operators
				int currentP = Precedence[current];
				int topP = Precedence[top];

				while((topP == 2 ||		// There is a function at the top 
					  topP > currentP ||
					  (topP == currentP && top != POW)) &&
					  top != LP)
				{ 
					output.push_back(stack.back());
					stack.pop_back();

					// Update top operator
					if(!stack.empty())
					{
						top = OperatorMap[stack.back()];
						topP = Precedence[top];
					} else
						break;
				}
			}
			stack.push_back(token);
		}
	}
	while(!stack.empty())	// Dump remaining operators from the stack onto the output 
	{
		output.push_back(stack.back());
		stack.pop_back();
	}

	return output;
}

// Evaluate the value of the function in postfix notation
float EvaluateFunction(std::vector<std::string>& function, float x)
{
	// Calculate expression
	std::vector<std::string> final;
	for(size_t i = 0; i < function.size(); i++)
	{
		std::string current = function[i];

		if(IsNumber(current))
			final.push_back(current);
		else if(current == "x" || current == "X")
			final.push_back(std::to_string(x));
		else
		{
			// Grab the top 2 operads on the stack
			std::string temp1;
			std::string temp2 = final.back();
			final.pop_back();
			if(!final.empty())
			{
				temp1 = final.back();
				final.pop_back();
			}

			double d = 0.0;
			switch(OperatorMap[current])
			{
			case ADD:
				d = atof(temp1.c_str()) + atoi(temp2.c_str());
				final.emplace_back(std::to_string(d));
				break;
			case SUB:
				d = atof(temp1.c_str()) - atof(temp2.c_str());
				final.emplace_back(std::to_string(d));
				break;
			case MUL:
				d = atof(temp1.c_str()) * atof(temp2.c_str());
				final.emplace_back(std::to_string(d));
				break;
			case DIV:
				d = atof(temp1.c_str()) / atof(temp2.c_str());
				final.emplace_back(std::to_string(d));
				break;
			case POW:
				d = pow(atof(temp1.c_str()), atof(temp2.c_str()));
				final.emplace_back(std::to_string(d));
				break;
			case SQRT:
				d = sqrt(atof(temp2.c_str()));
				final.push_back(temp1);	// Return the unused operand
				final.emplace_back(std::to_string(d));
				break;
			case SIN:
				d = sin(atof(temp2.c_str()));
				final.push_back(temp1);	// Return the unused operand
				final.emplace_back(std::to_string(d));
				break;
			case COS:
				d = cos(atof(temp2.c_str()));
				final.push_back(temp1);	// Return the unused operand
				final.emplace_back(std::to_string(d));
				break;
			case TAN:
				d = tan(atof(temp2.c_str()));
				final.push_back(temp1); // Return the unused operand
				final.emplace_back(std::to_string(d));
				break;
			case COT:
				d = 1.0 / tan(atof(temp2.c_str()));
				final.push_back(temp1); // Return the unused operand
				final.emplace_back(std::to_string(d));
				break;
			default:
				printf("Failed to parse expression and calculate value!");
			};
		}
	}

	return static_cast<float>(atof(final.back().c_str()));
}

// Rotate all points of a mesh around an axis
std::vector<float> RotateAroundAxis(std::vector<float>& vertices2D, int numRotations, char axis)
{
	// Increment size to rotate by (in degrees),
	// to reach 360 degrees with the number of
	// set iterations.
	float mul = 360.0f / numRotations;

	std::vector<float> vertices3D;
	for(size_t i = 0; i < vertices2D.size(); i += 3)
	{
		glm::vec3 current(vertices2D[i], vertices2D[i + 1], vertices2D[i + 2]);

		// Complete a full circle 
		for(int j = 0; j < numRotations; j++)
		{
			glm::mat4 rotator(1.0);

			if(axis == 'x' || axis == 'X')
			{
				// Rotate along the X axis
				rotator = glm::rotate(rotator, glm::radians(mul * j), { 1.0, 0.0, 0.0 });
			} else if(axis == 'y' || axis == 'Y')
			{
				// Rotate along the Y axis
				rotator = glm::rotate(rotator, glm::radians(mul * j), { 0.0, 1.0, 0.0 });
			} else if(axis == 2)
			{
				// Rotate along the Z axis
				rotator = glm::rotate(rotator, glm::radians(mul * j), { 0.0, 0.0, 1.0 });
			} else
			{
				printf("Axis %c is not supported!\n", axis);
			}

			// Rotated vertex
			glm::vec4 rotatedCurrent = rotator * glm::vec4(current, 1.0);

			vertices3D.push_back(rotatedCurrent.x);
			vertices3D.push_back(rotatedCurrent.y);
			vertices3D.push_back(rotatedCurrent.z);
		}
	}

	return vertices3D;
}

Mesh CreateRotatedMesh(std::vector<float>& vertices2D, int numRotations, char axis)
{
	// Generate rotated vertices along the circle
	// defined by the function's value as the radius.
	std::vector<float> vertices3D = RotateAroundAxis(vertices2D, numRotations, axis);

	// This is the number of actual vertices - the number of rotations
	// for each vertex. It's basically the number of faces.
	size_t numIterations = vertices3D.size() / 3 - numRotations - 1;

	// Store indices
	std::vector<unsigned int> indices;
	for(unsigned int i = 0; i < numIterations; i++)
	{
		// ...
		// 3 2 7 6 ...
		// 0 1 4 5 ...

		indices.push_back(i);
		indices.push_back(i + 1);
		indices.push_back(i + numRotations + 1);
		indices.push_back(i + numRotations + 1);
		indices.push_back(i + numRotations);
		indices.push_back(i);
	}

	// Calculate normal vectors
	std::vector<float> normals;
	size_t rotationsPerVertex = static_cast<size_t>(3) * numRotations;
	for(size_t i = 0; i < vertices3D.size() - rotationsPerVertex; i += 3)
	{
		/* The first of 2 triangles in each face
		   C
		   |\
		   | \
		   |  \
		   A---B
		*/

		glm::vec3 A{ vertices3D[i], vertices3D[i + 1], vertices3D[i + 2] };
		glm::vec3 B{ vertices3D[i + 3], vertices3D[i + 4], vertices3D[i + 5] };
		glm::vec3 C{ vertices3D[i + rotationsPerVertex], vertices3D[i + 1 + rotationsPerVertex], vertices3D[i + 2 + rotationsPerVertex] };

		// For the triangle ABC
		glm::vec3 u = B - A;
		glm::vec3 v = C - A;

		// The cross product is the vector that 
		// has a direction normal to the uv plane.
		glm::vec3 normal = glm::cross(u, v);

		normals.push_back(normal.x);
		normals.push_back(normal.y);
		normals.push_back(normal.z);
	}
	return Mesh(vertices3D, indices, normals);
}