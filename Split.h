#pragma once
/*#include <vector>
#include <string>*/

static const std::vector<std::string> explodeString(const std::string& s, const char& c)
{
	std::string buff{ "" };
	std::vector<std::string> v;

	for (auto n : s)
	{
		if (n != c) buff += n; else
			if (n == c && buff != "") { v.push_back(buff); buff = ""; }
	}
	if (buff != " ") v.push_back(buff);

	return v;
}

static const std::vector<int> explodeInt(const std::string& s, const char& c)
{
	std::string buff{ "" };
	std::vector<int> v;

	for (auto n : s)
	{
		if (n != c) buff += n; else
			if (n == c && buff != "") { v.push_back(std::stoi(buff)); buff = ""; }
	}
	if (buff != "") v.push_back(std::stoi(buff));

	return v;
}