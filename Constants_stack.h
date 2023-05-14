#pragma once

#include <string>;
#include <vector>;
#include "Variables.h";

class Constants_stack
{
	std::vector<Variables> constants{
		{ "PI", 3.1415926535 }
	};
public:
	const double get_constant(std::string name);
	const bool is_constant(std::string name);
	const void show_constants();
};

