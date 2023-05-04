#pragma once

#include <vector>
#include <string>
#include "Token.h"
#include "Variables_stream.h"

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

// ����� ��������� ������� � ���������� ���������.
class Token_stream {
private:
	// ������ �������, �� ������� ��������� ������� �������������� ���������.
	int index = 0;
	// ������� �������� �����.
	std::vector<Token> tokens;
	// ����������.
	Variables_stream vars;
	// ��������� ���������� ����������.
	bool declaration = false;
public:
	const std::string clear_whitespaces(std::string input);
	void parsing(std::string input);
	Token get();
	// ���������� ������ �� ���������� �������.
	void putback() { index--; }
	const void show_tokens();


	const bool get_declaration() { return declaration; }
	void set_declaration(bool declaration) { this->declaration = declaration; }
	const Variables_stream get_variable() { return vars; }
};
