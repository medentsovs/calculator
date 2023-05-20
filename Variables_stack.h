#pragma once
#include "Variables.h"
#include <string>
/*
#include <vector>
#include "errors.h"
*/

class Variables_stack {
	std::vector<Variables> variables{
		{ "PI",   3.1415926535, CONSTANTS, "" },
		{ "sqrt", 1,            FUNCTIONS, "(variable)\t\t���������� ������ �� �����." },
		{ "pow",  2,            FUNCTIONS, "(variable, degree)\t���������� ������� ��������� � ������� ������� ���������." }
	};

	// ����������, ���� �� � ���� ����������� ������������� ����������.
	// ���������� true, ���� ���������� ���� ����������, ����� ���������� false.
	const bool is_defined_variables() {
		for (Variables v : variables) 
			if (v.type == 'V') return true;
		return false;
	}
	// � ����������� �� ����������� ������� ���������� ��������� ��������� ����������.
	const std::string show_title_variables(char type) {
		switch (type) {
		case 'V':
			if (is_defined_variables()) return "����������� ����������:";
			else                        return "���������� �� ����������!";
		case 'C':                       return "����������� ���������:";
		case 'F':                       return "����������� �������:";
		default:                        throw ErrorDefinedVariables{};
		}
	}
	// � ����������� �� ����������� ������� ���������� ������ �������� ���������� ��� ����������� �������.
	const std::string show_footer_variables(char type, int index) {
		switch (type) {
		case 'V': case 'C': return " = " + std::to_string(variables[index].value);
		case 'F':           return variables[index].comment;
		default:            throw ErrorDefinedVariables{};
		}
	}
public:
	// � ����������� �� ����������� ������� ������� � ����������� ����� ������ ����������� � ������� ����������.
	const void show_variables(char type) {
		std::cout << show_title_variables(type) << std::endl;
		for (int i = 0; i < variables.size(); i++) {
			if (variables[i].type == type) {
				std::cout << variables[i].name;
				std::cout << show_footer_variables(type, i) << std::endl;
			}
		}
	}
	// ���� ���������� ��������� � �������, �� ���������� � ������, ����� ����� -1.
	int is_variable(std::string name) {
		for (int i = 0; i < variables.size(); i++) {
			if (variables[i].name == name) return i;
		}
		return -1;
	}
	// ���� ���������� ��� � �������, �� ��������� �.
	// ���� ���� � ���������� �� �������� ���������� ��� ��������, �� ��������� � ��������.
	// � ��������� ������ ����������� ����������.
	// ���������� ������ ��������� ��� ����������� ����������.
	int add_variable(std::string name, double value) {
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
			variables.push_back(Variables{name, value, VARIABLE, ""});
			return variables.size() - 1;
		}
	}
	// ���������� �������� ���������� �� � �����.
	// ���� ���������� �� ����������, �� ����� false.
	double get_variable(std::string name) {
		for (Variables v : variables) {
			if (v.name == name) {
				return v.value;
			}
		}
		return false;
	}
	// ���������� �������� ���������� �� � �������.
	double get_variable(int index) {
		return variables[index].value;
	}
	std::string get_name(int index) {
		return variables[index].name;
	}
	// ���������� ��� ���������� �� � �����.
	// ���� ���������� �� ����������, �� ����� false.
	char get_type(std::string name) {
		for (Variables v : variables) {
			if (v.name == name) {
				return v.type;
			}
		}
		return false;
	}










	/*
	const int isvariable(std::string name) {
		for (int i = 0; i < variables.size(); i++) {
			if (variables[i].name == name) return i;
		}
		return -1;
	}
	const std::string get_name(int index) {
		return variables[index].name;
	}
	const double get_value(std::string name) {
		for (int i = 0; i < variables.size(); i++) {
			if (variables[i].name == name) return variables[i].value;
		}
		return false;
	}
	const char get_type(std::string name) {
		for (int i = 0; i < variables.size(); i++) {
			if (variables[i].name == name) {
				return variables[i].type;
			}
		}
		return false;
	}
	const char get_type(int index) {
		return variables[index].type;
	}
	void add_variable(std::string name) {
		variables.push_back(Variables{ name, (double)variables.size(), 'V' });
	}

	const void show_variables(char type = 'V') {
		bool is_variables = false;
		switch (type) {
		case 'V':
			std::cout << "����������� ����������:" << std::endl;
			break;
		case 'C':
			std::cout << "����������� ���������:" << std::endl;
			break;
		case 'F':
			std::cout << "����������� �������:" << std::endl;
			break;
		default:
			throw ErrorDefinedVariables{};
		}
		for (Variables v : variables) {
			if (v.type == type) {
				std::cout << v.name << " = " << v.value << std::endl;
				is_variables = true;
			}
		}
		if (!is_variables) {
			std::cout << "���������� �� ����������." << std::endl;
		}
	}
	*/







	/*
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
	*/
};

Variables_stack vs;