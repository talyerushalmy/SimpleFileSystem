#include "../include/StringTools.h"

std::vector<std::string> StringTools::split(std::string s, std::string delimiter)
{
	std::vector<std::string> list;
	size_t pos = 0;
	std::string token;

	while ((pos = s.find(delimiter)) != std::string::npos)
	{
		token = s.substr(0, pos);
		if (token.length() != 0)
			list.push_back(token);
		s.erase(0, pos + delimiter.length());
	}

	list.push_back(s);

	return list;
}

std::string StringTools::trim(const std::string& str)
{
	size_t first = str.find_first_not_of(' ');
	if (std::string::npos == first)
	{
		return str;
	}
	size_t last = str.find_last_not_of(' ');
	return str.substr(first, (last - first + 1));
}