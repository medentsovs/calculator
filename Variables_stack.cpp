/*
#include <iostream>
#include <vector>
#include "Variables.h"
#include "Variables_stack.h"

const int Variables_stack::is_variable(std::string name) {
	if (!variables.empty()) {
		for (int i = 0; i < variables.size(); i++) {
			if (variables[i].name == name) return i;
		}
	}
	return -1;
}

void Variables_stack::add_variable(std::string name, double value) {
	if (is_variable(name) == -1) {
		Variables v = { name, value };
		variables.push_back(v);
		index++;
	}
	else {
		variables[is_variable(name)].value = value;
	}
	last_number = value;
}

const double Variables_stack::get_variable(int index) {
	if (variables.size() > 0)
		return variables[index].value;
	else
		throw ErrorNoVariable{};
}
const double Variables_stack::get_variable(std::string name) {
	if (is_variable(name) != -1)
		return variables[is_variable(name)].value;
	else
		throw ErrorNoVariable{};
}

void Variables_stack::show_variables() {
	if (variables.size() == 0) {
		std::cout << "Переменные не определены!" << std::endl;
		return;
	}
	std::cout << "Определённые переменные:" << std::endl;
	for (Variables v : variables) {
		std::cout << v.name << " = " << v.value << std::endl;
	}
}
*/