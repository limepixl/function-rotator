#pragma once
#include <string>
#include <vector>
#include <map>

enum Operator
{
	ADD = 0,
	SUB = 0,
	MUL = 1,
	DIV = 1,
	POW = 2,
	SQRT = 2,
	SIN = 3,
	COS = 3,
	LP,	// Left parenthesis
	RP	// Right parenthesis
};

std::map<std::string, Operator> OperatorMap;

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

void ParseFunction(std::string& function)
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
	OperatorMap["("] = LP;
	OperatorMap[")"] = RP;

	std::vector<std::string> split = Split(function, ' ');
	
	std::vector<std::string> output;
	std::vector<std::string> stack;

	for(size_t i = 0; i < split.size(); i++)
	{
		std::string token = split[i];
		if(IsNumber(token))
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
			while((OperatorMap[stack.back()] > current || (OperatorMap[stack.back()] == current && OperatorMap[stack.back()] != POW)) && OperatorMap[stack.back()] != LP)
			{
				output.push_back(stack.back());
				stack.pop_back();
			}

			stack.push_back(token);
		}
	}

	while(!stack.empty())
	{
		output.push_back(stack.back());
		stack.pop_back();
	}

}