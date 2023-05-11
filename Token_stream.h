#pragma once
#include <vector>
#include "Token.h"
#include "Variables.h"
#include "Variables_stack.h"

class Token_stream {
	// ������ �������, �� ������� ��������� ������� �������������� ���������.
	int index = 0;
	// ������� �������� �����.
	std::vector<Token> tokens;
	std::vector<Variables> vars;
public:
	const std::string clear_whitespaces(std::string input);
	void parsing(std::string input, Variables_stack& vs);
	Token get();
	// ���������� ������ �� ���������� �������.
	void putback() { index--; }
	const void show_tokens();
};













/*
#include <string>
#include "Variables_stream.h"
*/
/*
 * ���� ������������ ���������� ��������� �� �������:
 * �������� � ��������� ������ : 3.14, 0.274e2, 42
 * ���������, ����� ��� + , -, *, / , %
 * ������ : (, )
 * ������������ ������� - ������� ������ ������� � ���� ����(���, ��������).
 *  ��� ����������� � ������� ������ Token.
 * ��� �������������� ������� - ��� ��� ����� ������������ : �����, �������� ��� ������?
 *  ��� ����� � �������� �������� ������������ ���� �����.
 */
 /*
// ����� ��������� ������� � ���������� ���������.
class Token_stream {
private:
	
	
	// ����������.
	Variables_stream vars;
	// ��������� ���������� ����������.
	bool declaration = false;
public:
	
	
	
	


	const bool get_declaration() { return declaration; }
	void set_declaration(bool declaration) { this->declaration = declaration; }
	const Variables_stream get_variable() { return vars; }
};
*/