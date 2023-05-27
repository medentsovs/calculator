#pragma once
#include <iostream>
#include <vector>
#include "errors.h"
#include "constants.h"
#include "Token.h"
#include "Variables_stack.h"

/*
 * ���� ������������ ���������� ��������� �� �������:
 * �������� � ��������� ������: 3.14, 0.274e2, 42
 * ���������, ����� ��� + , -, *, / , %, <, >, =, ==
 * ������: (, ), {, }
 * ����������: var, Result, Variable
 * ���������: PI
 * �������: sqrt, !factorial
 * ������������ ������� - ������� ������ ������� � ���� ����(���, ��������).
 *  ��� ����������� � ������� ������ Token.
 * ��� �������������� ������� - ��� ��� ����� ������������: �����, �������� ��� ������?
 *  ��� ����� � �������� �������� ������������ ���� �����.
 *  ��� ����������, �������� � ������� - ������, �� �������� ��� ��������� � �������.
 */

extern bool isspecsymbol(char ch);
extern bool ishotkey(char ch);

class Token_stream {
	std::vector<Token> tokens;		// ����� �������� �������.
	std::vector<Variables> buffer;	// ����� �������� �������� ���������������� ����������, ������� ��� �� ����������������.
	std::string current = "";		// ���������� ��������.
	std::string pars;				// ������, �������� ������������� ��� �������� ������.
	int left_break = 0;				// ������� ����� ������.
	int right_break = 0;			// ������� ������ ������.
	int comparison = 0;				// ������� ���������� ���������.
	int index = 0;					// ������ ������ pars.

	const std::string clear_whitespaces(std::string input);
	const char number_or_variable(std::string str);
	void is_brackets();
	bool is_equally();
	bool is_comparison();
	const void operators();
	const void variable();
	void number();

public:
	void parsing(std::string input);
	Token get();
	void putback();
	const std::string get_buffer_name(int index);
	const void show_tokens();
	const int get_index();
};

