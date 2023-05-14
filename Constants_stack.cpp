#include <iostream>
#include "Constants_stack.h"

const double Constants_stack::get_constant(std::string name) {
	for (Variables v : constants) {
		if (v.name == name) {
			return v.value;
		}
	}
	return -1;
}

const bool Constants_stack::is_constant(std::string name) {
	for (Variables v : constants) {
		if (v.name == name) {
			return true;
		}
	}
	return false;
}

const void Constants_stack::show_constants() {
	std::cout << "Определённые константы:" << std::endl;
	for (Variables v : constants) {
		std::cout << v.name << " = " << v.value << std::endl;
	}
}