#include <iostream>
#include "Function.h"
#include "Functions_stack.h"

const bool Functions_stack::is_function(std::string str) {
	for (Function f : functions) {
		if (f.name == str) {
			return true;
		}
	}
	return false;
}

const int Functions_stack::get_index(std::string str) {
	for (int i = 0; i < functions.size(); i++) {
		if (functions[i].name == str) {
			return i;
		}
	}
	return -1;
}

const void Functions_stack::show_functions() {
	std::cout << "Определённые функции:" << std::endl;
	for (Function f : functions) {
		std::cout << f.name << "\t" << f.comment << std::endl;
	}
}