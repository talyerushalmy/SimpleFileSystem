#pragma once

#include <string>
#include <vector>

class StringTools
{
public:
	static std::vector<std::string> split(std::string phrase, std::string delimiter);
	static std::string trim(const std::string& input);
	static unsigned long hash(std::string str); // to use switch on strings
};
