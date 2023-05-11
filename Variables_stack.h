#pragma once
#include <vector>
#include <string>
#include "errors.h"
#include "Variables.h"

class Variables_stack {
	std::vector<Variables> variables;
	int index = 0;
	double last_number = 0;
public:
	const int is_variable(std::string name);
	void add_variable(std::string name, double value);
	const std::string get_name(int index) { return variables[index].name; }
	const double get_variable(int index);
	const double get_variable(std::string name);
	const int get_count() { return index; }
	const double get_last_number() { return last_number; }
	void show_variables();
};