#pragma once
#include <vector>
#include <string>
#include "Token.h"

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
public:
	const std::string clear_whitespaces(std::string input);
	void parsing(std::string input);
	Token get();
	// ���������� ������ �� ���������� �������.
	void putback() { index--; }
	const void show_tokens();
};