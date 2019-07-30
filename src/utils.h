#pragma once
#include <string>
#include <vector>

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

	std::vector<std::string> split = Split(function, ' ');
	
	std::vector<std::string> output;
	std::vector<std::string> stack;

	for(auto& c : split)
	{
		if(IsNumber(c))
			output.push_back(c);
		else
			stack.push_back(c);
	}

	for(auto& s : stack)
		printf("%s\n", s.c_str());
	
	for(auto& s : output)
		printf("%s\n", s.c_str());
}