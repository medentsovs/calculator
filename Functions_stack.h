#pragma once

#include <vector>
#include "Function.h"

class Functions_stack
{
	std::vector<Function> functions{
		{ "sqrt", "Корень из числа: sqrt(expression)", 1 }
	};
public:
	const bool is_function(std::string str);
	const int get_index(std::string str);
	const void show_functions();
};

