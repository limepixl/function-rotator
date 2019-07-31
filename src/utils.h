#pragma once
#include <string>
#include <vector>
#include <map>

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
	LP,	// Left parenthesis
	RP	// Right parenthesis
};

std::map<std::string, Operator> OperatorMap;
std::map<Operator, int> Precedence;

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
	OperatorMap["("] = LP;
	OperatorMap[")"] = RP;

	// Initialize precedence map
	Precedence[ADD] = 0;
	Precedence[SUB] = 0;
	Precedence[MUL] = 1;
	Precedence[DIV] = 1;
	Precedence[SIN] = 2;
	Precedence[COS] = 2;
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
			{
				Operator top = OperatorMap[stack.back()];

				// Get the precedence of the operators
				int currentP = Precedence[current];
				int topP = Precedence[top];

				while((topP == 2 ||		// There is a function at the top 
					  topP > currentP ||
					  topP == currentP && top != POW) &&
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

float EvaluateFunction(std::vector<std::string>& function)
{
	// Calculate expression
	std::vector<std::string> final;
	for(size_t i = 0; i < function.size(); i++)
	{
		std::string current = function[i];
		if(IsNumber(current))
			final.push_back(current);
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

			int r = 0;
			double d = 0.0;
			switch(OperatorMap[current])
			{
			case ADD:
				r = atoi(temp1.c_str()) + atoi(temp2.c_str());
				final.emplace_back(std::to_string(r));
				break;
			case SUB:
				r = atoi(temp1.c_str()) - atoi(temp2.c_str());
				final.emplace_back(std::to_string(r));
				break;
			case MUL:
				r = atoi(temp1.c_str()) * atoi(temp2.c_str());
				final.emplace_back(std::to_string(r));
				break;
			case DIV:
				r = atoi(temp1.c_str()) / atoi(temp2.c_str());
				final.emplace_back(std::to_string(r));
				break;
			case POW:
				r = static_cast<int>(pow(atoi(temp1.c_str()), atoi(temp2.c_str())));
				final.emplace_back(std::to_string(r));
				break;
			case SQRT:
				d = sqrt(atoi(temp2.c_str()));
				final.push_back(temp1);	// Return the unused operand
				final.emplace_back(std::to_string(d));
				break;
			case SIN:
				d = sin(atoi(temp2.c_str()));
				final.push_back(temp1);	// Return the unused operand
				final.emplace_back(std::to_string(d));
				break;
			case COS:
				d = cos(atoi(temp2.c_str()));
				final.push_back(temp1);	// Return the unused operand
				final.emplace_back(std::to_string(d));
				break;
			default:
				printf("Failed to parse expression and calculate value!");
			};
		}
	}

	return static_cast<float>(atof(final.back().c_str()));
}