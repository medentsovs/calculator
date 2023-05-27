#include "Variables.h"
#include "Variables_stack.h"

// ����������, ���� �� � ���� ����������� ������������� ����������.
// ���������� true, ���� ���������� ���� ����������, ����� ���������� false.
const bool Variables_stack::is_defined_variables() {
	for (Variables v : variables)
		if (v.type == 'V') return true;
	return false;
}

// type - ��� �������: ����������, ��������� ��� �������.
// � ����������� �� ����������� ������� ���������� ��������� ��������� ����������.
// ���� �������� ����������� �������, �� ����������� ����������.
const std::string Variables_stack::show_title_variables(char type) {
	switch (type) {
	case 'V':
		if (is_defined_variables()) return "����������� ����������:";
		else                        return "���������� �� ����������!";
	case 'C':                       return "����������� ���������:";
	case 'F':                       return "����������� �������:";
	default:                        throw ErrorDefinedVariables{};
	}
}

// type - ��� �������: ����������, ��������� ��� �������.
// index - ������ ���������� � ����� ����������.
// � ����������� �� ����������� ������� ���������� ������ �������� ���������� ��� ����������� �������.
const std::string Variables_stack::show_footer_variables(char type, int index) {
	switch (type) {
	case 'V': case 'C': return " = " + std::to_string(variables[index].value);
	case 'F':           return variables[index].comment;
	default:            throw ErrorDefinedVariables{};
	}
}

// type - ��� �������: ����������, ��������� ��� �������.
// � ����������� �� ����������� ������� ������� � ����������� ����� ������ ����������� � ������� ����������.
const void Variables_stack::show_variables(char type) {
	std::cout << show_title_variables(type) << std::endl;
	for (int i = 0; i < variables.size(); i++) {
		if (variables[i].type == type) {
			std::cout << variables[i].name;
			std::cout << show_footer_variables(type, i) << std::endl;
		}
	}
}

// name - �������� ����������.
// ���� ���������� ��������� � �������, �� ���������� � ������, ����� ����� -1.
int Variables_stack::is_variable(std::string name) {
	for (int i = 0; i < variables.size(); i++) {
		if (variables[i].name == name) return i;
	}
	return -1;
}

// name - �������� ����������.
// value - �������� ����������.
// ���� ���������� ��� � �������, �� ��������� �.
// ���� ���� � ���������� �� �������� ���������� ��� ��������, �� ��������� � ��������.
// � ��������� ������ ����������� ����������.
// ���������� ������ ��������� ��� ����������� ����������.
int Variables_stack::add_variable(std::string name, double value) {
	int index = is_variable(name);
	if (index != -1) {
		char type = variables[index].type;
		switch (type) {
		case VARIABLE:
			variables[index].value = value;
			return index;
		case CONSTANTS:
			throw ErrorConstantInitial{};
		case FUNCTIONS:
			throw ErrorFunctionInitial{};
		}
	}
	else {
		variables.push_back(Variables{ name, value, VARIABLE, "" });
		return variables.size() - 1;
	}
}

// name - �������� ����������.
// ���������� �������� ���������� �� � �����.
// ���� ���������� �� ����������, �� ����� false.
double Variables_stack::get_variable(std::string name) {
	for (Variables v : variables) {
		if (v.name == name)
			return v.value;
	}
	return false;
}

// index - ������ ���������� � ����� ����������.
// ���������� �������� ���������� �� � �������.
double Variables_stack::get_variable(int index) {
	return variables[index].value;
}

// index - ������ ���������� � ����� ����������.
// ���������� ��� ���������� �� � �������.
std::string Variables_stack::get_name(int index) {
	return variables[index].name;
}

// name - �������� ����������.
// ���������� ��� ���������� �� � �����.
// ���� ���������� �� ����������, �� ����� false.
char Variables_stack::get_type(std::string name) {
	for (Variables v : variables) {
		if (v.name == name)
			return v.type;
	}
	return false;
}

// ch - ������ ������� �����.
// ��������� ���� ������ ������� ����� � ���������� ��������.
void Variables_stack::add_roman(char ch) {
	roman += ch;
}

// ���������� ������� �����.
const std::string Variables_stack::get_roman() {
	return roman;
}

// �������� ��������� ������� �����.
// ���������� ���������� �����.
char Variables_stack::roman_decrement() {
	char last_number = roman[roman.size() - 1];
	roman.pop_back();
	return last_number;
}

// �������� ������� �����.
void Variables_stack::reset_roman() {
	roman = "";
}