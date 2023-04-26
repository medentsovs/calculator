#pragma once

#include <string>
#include <vector>
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
	void parsing(std::string input);
	Token get();
	void putback();
	void show_tokens();
};

// input - ������ ���������.
// ������� ������������ ��������� �� �������.
// ���� ������� �����, �� Token.kind = 8
// ����� Token.kind ����� ����� ������ ���������.
// ���� ��������� �������� ������������ �������, �� ������� �������� ���������� ErrorUnknownToken.
void Token_stream::parsing(std::string input) {
	std::string num = "";
	for (unsigned int i = 0; i < input.size(); i++) {
		switch (input[i]) {
		case '0': case '1': case '2': case '3': case '4': case '5': case '6': case '7': case '8': case '9': case '.': case ',':
			if (i == input.size() - 1 || (!isdigit(input[i + 1]) && input[i + 1] != '.')) {
				num += input[i];
				tokens.push_back({ '8', atof(num.c_str()) });
				num = "";
			}
			else {
				if (input[i] == '.') {
					num += ',';
					continue;
				}
				num += input[i];
			}
			break;
		case '+': case '-': case '*': case '/': case '%': case '(': case ')':
			tokens.push_back({ input[i], 0 });
			break;
		case ' ':
			break;
		default:
			throw ErrorUnknownToken{};
		}
	}
}
// ���������� ��������� ������� � ����������� ������.
// ���� ������ ������� �� ������� ������� �������, �� ���������� -1 � ������� ������ ������.
Token Token_stream::get() {
	// ������� �����������.
	if (index >= tokens.size()) {
		Token t;
		index = 0;
		t.kind = '#';
		t.value = -1;
		tokens.clear();
		return t;
	}
	// ������ ������ �������� ������� �������.
	if (index == 0 && (tokens[index].kind == '-')) {
		int num = tokens[++index].value;
		++index;
		Token t;
		t.kind = '|';
		t.value = num;
		return t;
	}
	return tokens[index++];
}
// ���������� ������ �� ���������� �������.
void Token_stream::putback() {
	index--;
}
// ����� ������ � �������.
void Token_stream::show_tokens() {
	for (Token t : tokens) {
		std::cout << t.kind << " = " << t.value << std::endl;
	}
}