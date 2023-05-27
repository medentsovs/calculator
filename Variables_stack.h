#pragma once
#include <string>
#include <vector>
#include "errors.h"
#include "constants.h"
#include "Variables.h"

class Variables_stack {
	std::vector<Variables> variables{
		{ "PI",    3.1415926535, CONSTANTS, "" },
		{ "E",     2.7182818284, CONSTANTS, "" },
		{ "sqrt",  1,            FUNCTIONS, "(variable)\t\tВозвращает корень из variable." },
		{ "pow",   2,            FUNCTIONS, "(variable, degree)\tВозведение variable в степень degree." },
		{ "roman", 1,            FUNCTIONS, "(variable)\t\tПереводит Римские цифры variable в арабские." }
	};
	std::string roman;	// Римские цифры для функции roman().

	const bool        is_defined_variables();
	const std::string show_title_variables(char type);
	const std::string show_footer_variables(char type, int index);
public:
	const void        show_variables(char type);
	int               is_variable(std::string name);
	int               add_variable(std::string name, double value);
	double            get_variable(std::string name);
	double            get_variable(int index);
	std::string       get_name(int index);
	char              get_type(std::string name);
	void              add_roman(char ch);
	const std::string get_roman();
	char              roman_decrement();
	void              reset_roman();
};