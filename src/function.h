#pragma once
#include <vector>
#include "utils.h"

void MathFunction(std::vector<float>& vertices, std::vector<std::string>& parsed, float a, float b, float incrementSize)
{
	for(float s = a; s <= b; s += incrementSize)
	{
		// Insert function here
		float x = s;
		float y = EvaluateFunction(parsed, x);;
		float z = 0.0f;
		
		vertices.push_back(x);
		vertices.push_back(y);
		vertices.push_back(z);
	}
}