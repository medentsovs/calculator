
#include <iostream>
#include <vector>
#include "Variables.h"
#include "Variables_stream.h"

const double Variables_stream::get_variable(char name) {
	return variables[is_variable(name)].value;
}
void Variables_stream::add_variable(char name, double value) {
	if (is_variable(name) == -1) {
		variables.push_back({ name, value });
	}
	else {
		variables[is_variable(name)].value = value;
	}
}
const int Variables_stream::is_variable(char name) {
	for (int i = 0; i < variables.size(); i++) {
		if (variables[i].name == name) return i;
	}
	return -1;
}
void Variables_stream::show_variables() {
	if (variables.size() == 0) {
		std::cout << "Переменные не определены!" << std::endl;
		return;
	}
	std::cout << "Определённые переменные:" << std::endl;
	for (Variables v : variables) {
		std::cout << v.name << " = " << v.value << std::endl;
	}
}
