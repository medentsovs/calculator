#pragma once

#include <vector>
#include "Variables.h"

class Variables_stream
{
	int index;
	std::vector<Variables> variables;
public:
	void parsing(std::string input);
	
	const double get_variable(char name);
	void add_variable(char name, double value);
	const int is_variable(char name);
	void show_variables();
};