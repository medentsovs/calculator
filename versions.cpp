/*************************************************************************************************************************************************/

/* VERSION 0.6.1
#include <iostream>
#include <string>
#include <vector>
#include <locale.h>

#include "errors.h"
#include "Token.h"
#include "Token_stream.h"

/*
 * Calculator
 * �����������, ������� �������� ��������� ������� ���������.
 * ����� ������ �������� 25.04.2023
 * version 0.6.1
 *
 * ����� ��������� ���� ����� ������� ���������.
 *  ???
 *
 * �������� ���� �������.
 *  �������� �������, �� ����������� �����������, ������� ����� ������ ������� �������,
 *   �������� �����: 2+3.1*4=14.4, 10*(20+30)-100=400, 1+2*3/4%5+(6-7*(8))=-47.5
 *
 * ��� ����������� ���.
 *  ��������� ������� ����� ���������, ����� ������ �������� ������, �������� � �� �������,
 *   � �����, �������� ������� ��������� ��������� � ������� ��������� ���������.
 *  ������� ������������ ������� Token � �������� ��� ������� � � ��������.
 *  ������ � ��������� ������� �� ����� Token_stream, ������� ������ ������ �������� ���������.
 *   ����� �������� ������ ������ ������� � ������������ ����� ������.
 *  ������� expression(), term(), primary() ��������� ���������� ���������, ��������� ����,
 *   � ���������� ��������� ���������.
 *  ������ ������� � ������������ ����� errors.
 *   ��� ������������ ���������� ��������� ������� ��������� � ������� � ���������� ������.
 *
 * ����� ������� ������������� � ������� ������.
 *  ������� ������ ������ ���� ����� �������, ���������� ����� � ����� ����������: + - / * % ( ) . ,
 *   ��������� ������ �������� ������ � ��������� � �� �����.
 *
 * ����� ���������� ���� ���� �� ������� � ����� �������� ��� �� ����������.
 *  �������� � ������� ������ ����! ��������� ���� �� ����� �� ������.
 *  ����� �� ����������� ������ ������� � ���������������� �������������.
 *  ���������� ���� �������� � ����������� ���������� m = 9; v = 7; v * m;
 *  � ��� �� ����� �� ����� �������� � ���������� �������������� �������.
 *  ��� ����� �������� �����!
* /
/*
* ����������� ��� �������:
* - ������� ��������� ����, ������� � ������ �������.
* - ����� ������ � ����� ������ ����.
* - �������� ������ � ������� � ������� � ��������.
* /

double expression();
double term();
double primary();

Token_stream ts;

int main() {
	setlocale(LC_ALL, "Russian");

	std::string input;

	while (true) {
		try {
			std::cout << "���������: ";
			getline(std::cin, input);
			if (!std::cin) throw ErrorInvalidInput{};
			ts.parsing(input);
			std::cout << "�����: " << expression() << std::endl;
		}
		catch (ErrorUnknownToken) {
			std::cerr << "������! ����������� �������!\n";
		}
		catch (ErrorDivisionZero) {
			std::cerr << "������! ������� �� ����!\n";
		}
		catch (ErrorInvalidInput) {
			std::cerr << "������! �������� ���� ���������!\n";
		}
	}

	return 0;
}

/*********************************************************************
*	���������� ���������:
* --------------------------------------------------------------------
* ���������:
*	����
*	��������� "+" ����	// ��������
*	��������� "-" ����	// ���������
* ����:
*	���������_���������
*	���� "*" ���������_���������	// ���������
*	���� "/" ���������_���������	// �������
*	���� "%" ���������_���������	// ������� (������� �� ������)
* ���������_���������:
*	�����
*	"(" ��������� ")"	// �����������
* �����:
*	�������_�_���������_������
********************************************************************** /

/******************************************** /
/* �������, ����������� ���������� ���������* /
// ������� ���������, �������� �� ������� ����������.
// � �������� � ����������� '+' � '-'.
double expression() {
	double left = term();
	Token t;
	while (true) {
		t = ts.get();
		switch (t.kind) {
		case '+':
			left += term();
			break;
		case '-':
			left -= term();
			break;
		default:
			return left;
		}
	}
}
// ������� ���������, �������� �� ������� ������.
// � �������� � ����������� '*', '/', '%'
double term() {
	double left = primary();
	Token t;
	while (true) {
		t = ts.get();
		switch (t.kind) {
		case '*':
			left *= primary();
			break;
		case '/':
		{
			double p = primary();
			if (p == 0) {
				throw ErrorDivisionZero{};
			}
			left /= p;
			break;
		}
		case '%':
		{
			double p = primary();
			double res = (int)left / (int)p;
			left = left - (res * p);
			break;
		}
		default:
			ts.putback();	// ������������ �� ���������� �������
			return left;
		}
	}
}
// ������� ���������, �������� �� ������� ��������� ����������.
double primary() {
	Token t = ts.get();
	switch (t.kind) {
	case '8':	// �����
		return t.value;
	case '|':	// ������������� �����
		return 0 - t.value;
	case '(':	// ��������� � �������
	{
		return expression();
	}
	default:	// ������������ �� ���������� �������
		ts.putback();
	}
}

/* �������, ����������� ���������� ���������* /
/********************************************/

/*************************************************************************************************************************************************/

/* VERSION 0.6
 * Calculator
 * �����������, ������� �������� ��������� ������� ���������.
 * ����� ������ �������� 14.04.2023 - 25.04.2023
 * version 0.6
 *
 * ����� ��������� ���� ����� ������� ���������.
 * �������� ���� �������.
 * ��� ����������� ���.
 * ����� ������� ������������� � ������� ������.
 *  ������� ������ ������ ���� ����� �������, ���������� ����� � ����� ����������. ��������� ������ �������� ������ � ��������� � �� �����.
 * ����� ���������� ���� ���� �� ������� � ����� �������� ��� �� ����������.
 *  ���������, � ������� ����������� ��������� ������ �� ��������! �� �������� �� ������ �� ��������� �������! ��������� ������ ���� ���� ����!
 *   ��� ������� ����� ����������� ��������� ������� get_token() � ����� Token!
 *  ���������� ���� �������� � ����������� ���������� m = 9; v = 7; v * m;
 *  � ��� �� ����� �� ����� �������� � ���������� �������������� �������.
 *  ��� ����� �������� �����!
* /
/*
* ������:
* �������� �������, �� ����������� �����������, ������� ����� ������ ������� �������,
*  �������� �����: 2+3.1*4=14.4, 10*(20+30)-100=400, 1+2*3/4%5+(6-7*(8))
* /
/*
* ����������� ��� �������:
* ���� ������������ ���������� ��������� �� �������:
*  �������� � ��������� ������: 3.14, 0.274e2, 42
*  ���������, ����� ��� +, -, *, /, %
*  ������: (, )
* ������������ ������� - ������� ������ ������� � ���� ���� (���, ��������).
*  ��� �������������� ������� - ��� ��� ����� ������������: �����, �������� ��� ������?
*  ��� ����� � �������� �������� ������������ ���� �����.
*
* - ������� ��������� ����, ������� � ������ �������.
* - ����� ������ � ����� ������ ����.
* - �������� ������ � ������� � ������� � ��������.
* /
/*
*	���������:
*
* while (not_finished) {
	read_a_line
	calculate		// ���������� ����������
	write_result
* }
*
* /
/*
*	���������� ���������:
* ���������:
*	����
*	��������� "+" ����	// ��������
*	��������� "-" ����	// ���������
* ����:
*	���������_���������
*	���� "*" ���������_���������	// ���������
*	���� "/" ���������_���������	// �������
*	���� "%" ���������_���������	// ������� (������� �� ������)
* ���������_���������:
*	�����
*	"(" ��������� ")"	// �����������
* �����:
*	�������_�_���������_������
* /

class ErrorDivisionZero {};
class ErrorUnknownToken {};
class ErrorInvalidInput {};

class Token {
public:
	char kind;
	double value;
};

std::vector<Token> tokens;

// input - ���������, ������� ���������� ���������.
std::vector<Token> parsing_token(std::string input) {
	std::vector<Token> tokens;
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
	return tokens;
}

void show_token(std::vector<Token> tokens);
double expression();
double term();
double primary();

Token get_token(bool negative = false) {
	static int i = 0;
	if (i >= tokens.size()) {
		Token t;
		i = 0;
		t.kind = '#';
		t.value = -1;
		return t;
	}
	if (negative == true)
		return tokens[i--];

	return tokens[i++];
}
double expression() {
	double left = term();
	Token t;
	while (true) {
		t = get_token();
		switch (t.kind) {
		case '+':
			left += term();
			break;
		case '-':
			left -= term();
			break;
		default:
			return left;
		}
	}
}
double term() {
	double left = primary();
	Token t;
	while (true) {
		t = get_token();
		switch (t.kind) {
		case '*':
			left *= primary();
			break;
		case '/':
		{
			double p = primary();
			if (p == 0) {
				throw ErrorDivisionZero{};
			}
			left /= p;
			break;
		}
		case '%':
		{
			double p = primary();
			double res = (int)left / (int)p;
			left = left - (res * p);
			break;
		}
		default:
			get_token(true);
			return left;
		}
	}
}
double primary() {
	Token t = get_token();
	switch (t.kind) {
	case '8':
		return t.value;
	case '(':
	{
		return expression();
	}
	default:
		get_token(true);
	}
}

int main() {
	setlocale(LC_ALL, "Russian");

	std::string input;

	//input = "1+2+3";					// 6
	//input = "1-2-3";					// -4
	//input = "5 + 5 * 10";				// 55
	//input = "10*10+20*4+6";			// 186
	//input = "10+10*20/4-6";			// 54
	//input = "5+5*5/5-5";				// 5
	//input = "2+3.1*4";				// 14.4
	//input = "10*(20+30)-100";			// 400
	//input = "10 + (10 + 10)";			// 30
	//input = "1+2*3/4%5+(6-7*(8))";	// -50
	while (true) {
		try {
			std::cout << "���������: ";
			getline(std::cin, input);
			//std::cin >> input;
			if (!std::cin) throw ErrorInvalidInput{};
			tokens.clear();
			tokens = parsing_token(input);

			std::cout << "�����: " << expression() << std::endl;
		}
		catch (ErrorUnknownToken) {
			std::cerr << "������! ����������� �������!\n";
		}
		catch (ErrorDivisionZero) {
			std::cerr << "������! ������� �� ����!\n";
		}
		catch (ErrorInvalidInput) {
			std::cerr << "������! �������� ���� ���������!\n";
		}
	}

	return 0;
}

void show_token(std::vector<Token> tokens) {
	for (Token t : tokens) {
		std::cout << "'" << t.kind << "'\t";
	}
	std::cout << std::endl;
	for (Token t : tokens) {
		if (t.kind != '8') {
			std::cout << '_';
			std::cout << '\t';
			continue;
		}
		std::cout << t.value << '\t';
	}
	std::cout << std::endl;
}
*/

/*************************************************************************************************************************************************/

/* VERSION 0.5
#include <iostream>
#include <vector>
#include <string>
#include <locale.h>

/*
 * Calculator
 * �����������, ������� �������� ��������� ������� ���������.
 * ����� ������ �������� 08.04.2023-14.04.2023
 * version 0.5
 *
 * ����� ��������� ���� ����� ������� ���������.
 * �������� ���� �������.
 * ��� ����������� ���.
 * ����� ������� ������������� � ������� ������.
 *  ������� ������ ������ ���� ����� �������, ���������� ����� � ����� ����������. ��������� ������ �������� ������ � ��������� � �� �����.
 * ����� ���������� ���� ���� �� ������� � ����� �������� ��� �� ����������.
 *  ���������� ���� �������� � ����������� ���������� m = 9; v = 7; v * m;
 *  � ��� �� ����� �� ����� �������� � ���������� �������������� �������.
 *  ��� ����� �������� �����!
* /
/*
* ������:
* �������� �������, �� ����������� �����������, ������� ����� ������ ������� �������,
*  �������� �����: 2+3.1*4=14.4, 10*(20+30)-100=400, 1+2*3/4%5+(6-7*(8))
* /
/*
* ����������� ��� �������:*
* ���� ������������ ���������� ��������� �� �������:
*  �������� � ��������� ������: 3.14, 0.274e2, 42
*  ���������, ����� ��� +, -, *, /, %
*  ������: (, )
* ������������ ������� - ������� ������ ������� � ���� ���� (���, ��������).
*  ��� �������������� ������� - ��� ��� ����� ������������: �����, �������� ��� ������?
*  ��� ����� � �������� �������� ������������ ���� �����.
*
* - ������� ��������� ����, ������� � ������ �������.
* - ����� ������ � ����� ������ ����.
* - �������� ������ � ������� � ������� � ��������.
* /
/*
*	���������:
*
* while (not_finished) {
	read_a_line
	calculate		// ���������� ����������
	write_result
* }
*
* /
/*
*	���������� ���������:
* ���������:
*	����
*	��������� "+" ����	// ��������
*	��������� "-" ����	// ���������
* ����:
*	���������_���������
*	���� "*" ���������_���������	// ���������
*	���� "/" ���������_���������	// �������
*	���� "%" ���������_���������	// ������� (������� �� ������)
* ���������_���������:
*	�����
*	"(" ��������� ")"	// �����������
* �����:
*	�������_�_���������_������
* /
class Error {};
class Token {
public:
	char kind;
	double value;
};
// tokens - ������ ������, ������� ���������� �������.
// tokens ������� ����� tokens {kind: ..., value: ...}
void show_token(std::vector<Token> tokens) {
	for (Token t : tokens) {
		std::cout << "'" << t.kind << "'\t";
	}
	std::cout << std::endl;
	for (Token t : tokens) {
		if (t.kind != '8') {
			std::cout << '_';
			std::cout << '\t';
			continue;
		}
		std::cout << t.value << '\t';
	}
	std::cout << std::endl;
}
// input - ���������, ������� ���������� ���������.
// tokens - ������ ������, �� ������� ���������� ��������� ���������.
void parsing_token(std::string input, std::vector<Token>& tokens) {
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
		default:
			throw Error{};
		}
	}
}
int main() {
	setlocale(LC_ALL, "russian");

	std::string input;
	std::vector<Token> tokens;

	input = "10*10+20*4+6";	// 186
	//input = "10+10*20/4-6";	// 54
	//input = "5+5*5/5-5";		// 5
	//input = "2+3.1*4";		// 14.4
	//input = "10*(20+30)-100";	// 400
	input = "1+2*3/4%5+(6-7*(8))";

	parsing_token(input, tokens);

	return 0;
}
// brackets - ������ ��������� ������� ������� Token, � ������� ��������� ������ ().
void show_brackets(std::vector<int> brackets) {
	for (int i = 0; i < brackets.size(); i++) {
		std::cout << brackets[i] << " ";
	}
}
// tokens - ������ ������, ������������ � ���������.
// index - ��������� ������ ������� tokens, � �������� ���������� ������ ��������.
// end - �������� ������, ������� ���������� ��������� ��������.
void shift_token(std::vector<Token>& tokens, int index, int end = -1) {
	if (end == -1) end = tokens.size() - 2;
	for (unsigned int i = index; i < end; i++) {
		tokens[i] = tokens[i + 2];
	}
	tokens.pop_back();
	tokens.pop_back();
}
// tokens - ������ ������.
double order_execution(std::vector<Token>& tokens, int start = 0, int end = -1) {
	int count = end;
	if (end == -1) {
		count = tokens.size();
	}
	for (int i = start; i < (count - 1); i++) {
		switch (tokens[i].kind) {
		case '*':
			tokens[i - 1].value *= tokens[i + 1].value;
			shift_token(tokens, i);
			count -= 2;
			i -= 2;
			break;
		case '/':
			tokens[i - 1].value /= tokens[i + 1].value;
			shift_token(tokens, i);
			count -= 2;
			i -= 2;
			break;
		}
	}
	for (int i = start; i < (count - 1); i++) {
		switch (tokens[i].kind) {
		case '+':
			tokens[i - 1].value += tokens[i + 1].value;
			shift_token(tokens, i);
			count -= 2;
			i -= 2;
			break;
		case '-':
			tokens[i - 1].value -= tokens[i + 1].value;
			shift_token(tokens, i);
			count -= 2;
			i -= 2;
			break;
		}
	}
	return tokens[0].value;
}
std::vector<int> find_brackets(std::vector<Token> tokens) {
	std::vector<int> indexes;
	for (unsigned int i = 0; i < tokens.size(); i++) {
		if (tokens[i].kind == '(' || tokens[i].kind == ')') {
			indexes.push_back(i);
		}
	}
	return indexes;
}

*/

/*************************************************************************************************************************************************/

	/*
	* VERSION 0.4
	* #include <iostream>
	#include <vector>
	#include <string>
	#include <locale.h>
	/*
	 * Calculator
	 * �����������, ������� �������� ��������� ������� ���������.
	 * ����� ������ �������� 06.04.2023-07.04.2023
	 * version 0.4
	 *
	 * ����� ��������� ���� ����� ������� ���������.
	 * �������� ���� �������.
	 * ��� ����������� ���.
	 * ����� ������� ������������� � ������� ������.
	 *  ������� ������ ������ ���� ����� �������, ���������� ����� � ����� ����������. ��������� ������ �������� ������ � ��������� � �� �����.
	 * ����� ���������� ���� ���� �� ������� � ����� �������� ��� �� ����������.
	 *  ���������� ���� �������� � ����������� ���������� m = 9; v = 7; v * m;
	 *  � ��� �� ����� �� ����� �������� � ���������� �������������� �������.
	 *  ��� ����� �������� �����!
	* /
	/*
	* ������:
	* �������� �������, �� ����������� �����������, ������� ����� ������ ������� �������,
	*  �������� �����: 2+3.1*4=14.4, 10*(20+30)-100=400, 1+2*3/4%5+(6-7*(8))
	* /
	/*
	* ����������� ��� �������:
	* �������� �� ��������, ������� � ���� �� ������� ������.
	* ��� ����� ��������, ��� �������� ������, ����� ��� 2+*3 � 2&3!
	*
	* ������ ���������� ����� �� �������� �� ��������. ����� ��������, ��� ����� �� �����������!
	*
	* ���� ������������ ���������� ��������� �� �������:
	*  �������� � ��������� ������: 3.14, 0.274e2, 42
	*  ���������, ����� ��� +, -, *, /, %
	*  ������: (, )
	* ������������ ������� - ������� ������ ������� � ���� ���� (���, ��������).
	*  ��� �������������� ������� - ��� ��� ����� ������������: �����, �������� ��� ������?
	*  ��� ����� � �������� �������� ������������ ���� �����.
	*
	* - ������� ��������� ����, ������� � ������ �������.
	* - ����� ������ � ����� ������ ����.
	* - �������� ������ � ������� � ������� � ��������.
	* /
	/*
	*	���������:
	*
	* while (not_finished) {
		read_a_line
		calculate		// ���������� ����������
		write_result
	* }
	*
	* /
	/*
	*	���������� ���������:
	* ���������:
	*	����
	*	��������� "+" ����	// ��������
	*	��������� "-" ����	// ���������
	* ����:
	*	���������_���������
	*	���� "*" ���������_���������	// ���������
	*	���� "/" ���������_���������	// �������
	*	���� "%" ���������_���������	// ������� (������� �� ������)
	* ���������_���������:
	*	�����
	*	"(" ��������� ")"	// �����������
	* �����:
	*	�������_�_���������_������
	* /
	class Error {};
	class Token {
	public:
		char kind;
		double value;
	};
	// input - ���������, ������� ���������� ���������.
	// tokens - ������ ������, �� ������� ���������� ��������� ���������.
	void parsing_token(std::string input, std::vector<Token>& tokens) {
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
			default:
				throw Error{};
			}
		}
	}
	// tokens - ������ ������, ������� ���������� �������.
	// tokens ������� ����� tokens {kind: ..., value: ...}
	void show_token(std::vector<Token> tokens) {
		for (Token t : tokens) {
			std::cout << "'" << t.kind << "'\t";
		}
		std::cout << std::endl;
		for (Token t : tokens) {
			if (t.kind != '8') {
				std::cout << '\t';
				continue;
			}
			std::cout << t.value << '\t';
		}
		std::cout << std::endl;
	}
	// brackets - ������ ��������� ������� ������� Token, � ������� ��������� ������ ().
	void show_brackets(std::vector<int> brackets) {
		for (int i = 0; i < brackets.size(); i++) {
			std::cout << brackets[i] << " ";
		}
	}
	// tokens - ������ ������, ������������ � ���������.
	// index - ��������� ������ ������� tokens, � �������� ���������� ������ ��������.
	// end - �������� ������, ������� ���������� ��������� ��������.
	void shift_token(std::vector<Token>& tokens, int index, int end = -1) {
		if (end == -1) end = tokens.size() - 2;
		for (unsigned int i = index; i < end; i++) {
			tokens[i] = tokens[i + 2];
		}
		tokens.pop_back();
		tokens.pop_back();
	}
	// tokens - ������ ������.
	double order_execution(std::vector<Token>& tokens, int start = 0, int end = -1) {
		int count = end;
		if (end == -1) {
			count = tokens.size();
		}
		for (int i = start; i < (count - 1); i++) {
			switch (tokens[i].kind) {
			case '*':
				tokens[i - 1].value *= tokens[i + 1].value;
				shift_token(tokens, i);
				count -= 2;
				i -= 2;
				break;
			case '/':
				tokens[i - 1].value /= tokens[i + 1].value;
				shift_token(tokens, i);
				count -= 2;
				i -= 2;
				break;
			}
		}
		for (int i = start; i < (count - 1); i++) {
			switch (tokens[i].kind) {
			case '+':
				tokens[i - 1].value += tokens[i + 1].value;
				shift_token(tokens, i);
				count -= 2;
				i -= 2;
				break;
			case '-':
				tokens[i - 1].value -= tokens[i + 1].value;
				shift_token(tokens, i);
				count -= 2;
				i -= 2;
				break;
			}
		}
		return tokens[0].value;
	}
	std::vector<int> find_brackets(std::vector<Token> tokens) {
		std::vector<int> indexes;
		for (unsigned int i = 0; i < tokens.size(); i++) {
			if (tokens[i].kind == '(' || tokens[i].kind == ')') {
				indexes.push_back(i);
			}
		}
		return indexes;
	}
	int main() {
		setlocale(LC_ALL, "russian");
		std::string input;
		std::vector<Token> tokens;
		//while (true) {
			//std::cout << "���������: ";
		std::cin >> input;
		//input = "10*10+20*4+6";	// 186
		//input = "10+10*20/4-6";	// 54
		//input = "5+5*5/5-5";		// 5
		//input = "2+3.1*4";		// 14.4
		//input = "10*(20+30)-100";	// 400
		//input = "1+2*3/4%5+(6-7*(8))";
		try {
			parsing_token(input, tokens);
			std::vector<int> brackets = find_brackets(tokens);
			if (brackets.size() != 0) {
				order_execution(tokens, brackets[brackets.size() / 2 - 1], brackets[brackets.size() / 2]);
				show_brackets(brackets);
			}
			//show_token(tokens);
			std::cout << "�����: " << order_execution(tokens) << std::endl;
			//tokens.clear();
		}
		catch (Error) {
			std::cerr << "Error!!!\n";
		}
		//}
		return 0;
	}
	*/
	/*************************************************************************************************************************************************/
	/* VERSION 0.3
	 * Calculator
	 * �����������, ������� �������� ��������� ������� ���������.
	 * ����� ������ �������� 04.04.2023 - 05.04.2023
	 * version 0.3
	 *
	 * ����� ��������� ���� ����� ������� ���������.
	 *  ��������� �� ��������. ������ ������������ �������!
	 * �������� ���� �������.
	 * ��� ����������� ���.
	 * ����� ������� ������������� � ������� ������.
	 *  ������� ������ ������ ���� ����� �������, ���������� ����� � ����� ����������. ��������� ������ �������� ������ � ��������� � �� �����.
	 * ����� ���������� ���� ���� �� ������� � ����� �������� ��� �� ����������.
	 *  ������� parsing - ����� � ���������� ������� �������� �� ������������ ����� ������ � ����������.
	 *  �� ���� �������, ����� ������ ��������� �������� ������� �����!
	 *
	 * �������� ����� ��������:
	 *  ��� ����� ����� ������������� ������ �� �������� ��������� ���������� ���������� � ��������� �� � ������ order.
	 *  �������� ����������� 1 2 0 3.
	 *  ���������� ������ ���������� � ������� 1 � 1+1, ��������� ��������� � ������ 1, � � ������ 1+1 ��������� ����� �� ������ ��������� �� 1+1.
	 *  �� ���� ������� ��� �����, � ��������� ������ �������. ����� � ������� order � ��� ���������� ������ ������������������. ��� ������ �� ��������� �� 1:
	 *  �� ���� ������ � ������� order ����� 1 0 2, ������ ������ �� ������. � ��� �����, � ������� order ��������� �������� ����� 0 1 � 0.
	 *  � � ������� ����� ����� ���������� �� ������ �����, ���� �� ��������� ���� - ��� � ����� ��� �����!
	* /
	/*
	* ������:
	* �������� �������, �� ����������� �����������, ������� ����� ������ ������� �������,
	*  �������� �����: 2+3.1*4=14.4, 10*(20+30)-100=400
	* /
	/*
	* ����������� ��� �������:
	*
	* ���� ������������ ���������� ��������� �� �������:
	*  �������� � ��������� ������: 3.14, 0.274e2, 42
	*  ���������, ����� ��� +, -, *, /, %
	*  ������: (, )
	* ������������ ������� - ������� ������ ������� � ���� ���� (���, ��������).
	*  ��� �������������� ������� - ��� ��� ����� ������������: �����, �������� ��� ������?
	*  ��� ����� � �������� �������� ������������ ���� �����.
	* (_��� �������� ����������� ���� ��������?_)
	*  ������� ����� ���������� � ����� ������������������ ����������� ���������.
	*   ��� ����� ������� � ������� ������� find_multi_division find_sum_diff
	*  ����� ������ ��������� ��������� ��������� ��������� ������ � �������.
	*  ����� ��������� � ������ ������, � ��� ��������� ������ ����� �������, ������ ������ ��������������.
	*  ������ � ������� ��� �������� �� �������� ����������� � ������� ������: [index * 2 - index, index * 2 + 1 - index]
	*
	* ���������� ������� ���������� ��� �� �� ������������� �������� ����������� ��������. ����� ���_�� ������� �� � ����.
	*
	* - ������� ��������� ����, ������� � ������ �������.
	* - ����� ������ � ����� ������ ����.
	* - �������� ������ � ������� � ������� � ��������.
	* /
	/*
	*	���������:
	*
		read_a_line
		calculate		// ���������� ����������
		write_result
	*
	* /
	// ������ �������� ������������� ������ � ��������� � �� ����� � ���������.
	// �������� str: ��� � ���� ������ �������� �������������.
	// �������� opers: ������ �� ������, � ������� ����������� ���������.
	// �������� nums: ������ �� ������, � ������� ����������� �����.
	// �������� ������� �� ������, ���� ����� ���� ��.
	void parsing(std::string str, std::vector<char>& opers, std::vector<double>& nums) {
		std::string num = "";
		for (unsigned int i = 0; i < str.size(); i++) {
			switch (str[i]) {
			case '0': case '1': case '2': case '3': case '4': case '5': case '6': case '7': case '8': case '9':
				if ((i == str.size() - 1) || !(isdigit(str[i + 1]))) {
					num += str[i];
					nums.push_back(atof(num.c_str()));
					num = "";
				}
				else {
					num += str[i];
				}
				break;
			case '+': case '-': case '*': case '/':
				opers.push_back(str[i]);
			}
		}
	}
	// ���������� ������ ��������� ��������� � �������, �������� � ������� opers.
	// ����� ���������� � ������� start.
	// ���� ��������� ��������� � ������� �� �������, ������� ����� -1.
	int find_multi_division(std::vector<char> opers, int start) {
		for (unsigned int i = start; i < opers.size(); i++) {
			if (opers[i] == '*' || opers[i] == '/') {
				return i;
			}
		}
		return -1;
	}
	// ���������� ������ ��������� �������� � ���������, �������� � ������� opers.
	// ����� ���������� � ������� start.
	// ���� ��������� �������� � ��������� �� �������, ������� ����� -1.
	int find_sum_diff(std::vector<char> opers, int start) {
		for (unsigned int i = start; i < opers.size(); i++) {
			if (opers[i] == '+' || opers[i] == '-') {
				return i;
			}
		}
		return -1;
	}
	void determine_order(std::vector<char> opers, std::vector<int>& order) {
		int index = -1;
		while (true) {
			index = find_multi_division(opers, ++index);
			if (index == -1) break;
			order.push_back(index);
		}
		index = -1;
		while (true) {
			index = find_sum_diff(opers, ++index);
			if (index == -1) break;
			order.push_back(index);
		}
	}
	void shift(std::vector<int>& order, std::vector<double>& nums, int index) {
		for (unsigned int j = order[index]; j < order.size() - 1; j++) {
			std::cout << "ORDER: " << order[index] << std::endl;
			std::cout << j << " - NUMS[j] = " << nums[j] << ", " << j + 1 << " - NUMS[j + 1] = " << nums[j + 1] << std::endl;
			nums[j] = nums[j + 1];
		}
		nums.pop_back();
		/*for (unsigned int j = 0; j < order.size(); j++) {
			--order[j];
			if (order[j] < 0)
				order[j] = 0;
		}* /
	}
	* /
	int main() {
		setlocale(LC_ALL, "russian");

		std::string input;
		std::vector<char> arr_opers;
		std::vector<double> arr_nums;
		std::vector<int> arr_order;

		//std::cin >> input;
		//input = "10*10+20*4+6";	// 186
		input = "10+10*20/4-6";	// 54
		//input = "5+5*5/5-5";		// 5
		//input = "2+3.1*4";		// 14.4
		//input = "10*(20+30)-100";	// 400

		parsing(input, arr_opers, arr_nums);
		determine_order(arr_opers, arr_order);
		for (unsigned int i = 0; i < arr_order.size(); i++) {
			switch (arr_opers[arr_order[i]]) {
			case '+':
				arr_nums[arr_order[i] * 2 - arr_order[i]] += arr_nums[arr_order[i] * 2 + 1 - arr_order[i]];
				shift(arr_order, arr_nums, i);
				break;
			case '-':
				arr_nums[arr_order[i] * 2 - arr_order[i]] -= arr_nums[arr_order[i] * 2 + 1 - arr_order[i]];
				shift(arr_order, arr_nums, i);
				break;
			case '*':
				arr_nums[arr_order[i] * 2 - arr_order[i]] *= arr_nums[arr_order[i] * 2 + 1 - arr_order[i]];
				shift(arr_order, arr_nums, i);
				break;
			case '/':
				arr_nums[arr_order[i] * 2 - arr_order[i]] /= arr_nums[arr_order[i] * 2 + 1 - arr_order[i]];
				shift(arr_order, arr_nums, i);
				break;
			}
		}

		std::cout << "�����: " << arr_nums[0] << std::endl;

		return 0;
	}
	*/

	/*************************************************************************************************************************************************/

	/* VERSION 0.2
	 * Calculator
	 * �����������, ������� �������� ��������� ������� ���������.
	 * ����� ������ �������� 01.04.2023 - 04.04.2023
	 * version 0.2
	 *
	 * ����� ��������� ���� ����� ������� ���������.
	 * �������� ���� �������.
	 * ��� ����������� ���.
	 * ����� ������� ������������� � ������� ������.
	 *  ������� ������ ������ ���� ����� �������, ���������� ����� � ����� ����������. ��������� ������ �������� ������ � ��������� � �� �����.
	 * ����� ���������� ���� ���� �� ������� � ����� �������� ��� �� ����������.
	 *  ������� parsing - ����� � ���������� ������� �������� �� ������������ ����� ������ � ����������.
	 *
	 *  �����, �� �����������, ��� ��������� ����� ���������� ������� ��������� � �������, � ����� �������� � ���������.
	 *  �������� � ���, ��� ����� ��� ���� ������ ��� �������� �������� ��� ������ � ������������ ����������,
	 *  ����� ��������� ��������� �� ������ � ��������� �������, � �� ���� �������������� �������.
	 *  � �� ���� ��������� ������ ��������� �� ������ ���������.
	 *   �� � ����� ���������� ������� �� ����� ��������� ��������. �������� ��� � ���:
	 *    ���� ��������� ��� ����� ���������, �� �� ������������ ������, ��
	 *    ���� ������� ��� ��������, � ����� ���������, �� ����� ��������, ������ ��� � ����� �� ��������� ����� ������� ������ �����, ��
	 *    ���� ��������� ��� �����, �� �� ����� �������� ���������, � ������� ��������� ��� ������.
	 *   ��� ������� ����� �����������...
	 *  �� ���� ���� ��� ����� �����������! ����� ����� ������! �� ���������� ��� ������!!! ���� ���� ����������.
	 *
	 * �������� ����� ��������:
	 *  ��� ����� ����� ������������� ������ �� �������� ��������� ���������� ���������� � ��������� �� � ������ order.
	 *  �������� ����������� 1 2 0 3.
	 *  ���������� ������ ���������� � ������� 1 � 1+1, ��������� ��������� � ������ 1, � � ������ 1+1 ��������� ����� �� ������ ��������� �� 1+1.
	 *  �� ���� ������� ��� �����, � ��������� ������ �������. ����� � ������� order � ��� ���������� ������ ������������������. ��� ������ �� ��������� �� 1:
	 *  �� ���� ������ � ������� order ����� 1 0 2, ������ ������ �� ������. � ��� �����, � ������� order ��������� �������� ����� 0 1 � 0.
	 *  � � ������� ����� ����� ���������� �� ������ �����, ���� �� ��������� ���� - ��� � ����� ��� �����!
	* /
	/ *
	* ������:
	* �������� �������, �� ����������� �����������, ������� ����� ������ ������� �������,
	*  �������� �����: 2+3.1*4=14.4, 10*(20+30)-100=400
	* /
	/*
	* ����������� ��� �������:
	* ��� ������ ������ ���������?
	*  ��������� ������ ��������� ��������� �������� � ������� ���������.
	* ��� �� � ����� ����������������� � ����������?
	*  � ����� ��������� � �������, ��� Enter, � ��������� ����� ��� ��������� ����������.
	* ��� �������� ����� ���������?
	*  ��������� ������� � ���, ��� ��� ����� ��������� ��������� ����� �������� ��������� � �� ����.
	*  ������ ����������, ����� ��������� �������� ���������� ����� ����, ��� ����� ������� �� ���������.
	*  ��� ��������, ��� ��� ����� ������ �������� ������, ������� ������ ��������� ������� ��� ������ �� ������������.
	*  (_��� ������� ������? ��� �� ������ ������?_)
	*   ������ ������ ��������� ������ ����������� � �������� ����� � ���� ������, � ��������� � ������ ������.
	*   (_������ ���������, ��� ������� ����� �������, � �� �� ����� �����?_)
	*    ��� ���������� �������� ���������, ��������� ������ �������� ������ ��� ����������
	*    � ���� �������� ����������, �� ��������� � ������ �������.
	*   (_�� ��� �� ����� ����� ����� � ������ ��������� ���������?_)
	*    ������ �������� ����� ��� ����� ����� � ������ �� ����. ���� ���������� ��������� � ������ �� �������,
	*    ������� ��� ���������� ����� ����������� � ��������� �����: [index * 2 - index, index * 2 + 1 - index]
	*   ����� ��� ����� ����� ������� ��������� ��������� � ������� � �� ���������,
	*   ���������� � ��������� ��������� � ����� �������, ����������� � ���������.
	*   ��� �������� ��� ��������� ��������� �������� � ���������.
	*   ������ ����� ����� ������� ��� ���� ������, ������� ����� ��������� ������� ����������,
	*   ������� ��� ��������� ���� ������, ��� ���� ����� � �� ������� ��������� ������������ ����� � ������� �����,
	*   � ������� ��� ���������, ��� ����, ����� ��������� ��������� ����������� ���������.
	*   (_�� ��� � ����� ������ �������� �� ��������?_)
	*    ����� ��, ����� ������ ���������� �� ������.
	*    (_��, ���� ������ ����� ����������?_)
	*     ������ ����� ������ ������� ����� ��������� ������, ��������� ��� ���������, ������� ��������� � ���,
	*     � ����� ��������� �������� ������, � ����� ��� �� ���������.
	*     (_��� �� ��� �����������?_)
	*      �� ���� ����� ����������� �������, ������� ����� ��������� ������.
	*     � ����� ������� ����� ����������� �������� ������� ��� ������, � ����� ���������� � ������,
	*     ��� ��� ������ ��������� �� ��, ������ � ����� �������� ��������� � �������.
	*
	* - ������� ��������� ����, ������� � ������ �������.
	* - ����� ������ � ����� ������ ����.
	* - �������� ������ � ������� � ������� � ��������.
	* /
	/*
	*	���������:
	*
		read_a_line
		calculate		// ���������� ����������
		write_result
	*
	* /
	/*
	// ������ �������� ������������� ������ � ��������� � �� ����� � ���������.
	// �������� str: ��� � ���� ������ �������� �������������.
	// �������� opers: ������ �� ������, � ������� ����������� ���������.
	// �������� nums: ������ �� ������, � ������� ����������� �����.
	// �������� ������� �� ������, ���� ����� ���� ��.
	void parsing(std::string str, std::vector<char>& opers, std::vector<double>& nums) {
		std::string num = "";
		for (unsigned int i = 0; i < str.size(); i++) {
			switch (str[i]) {
			case '0': case '1': case '2': case '3': case '4': case '5': case '6': case '7': case '8': case '9':
				if ((i == str.size() - 1) || !(isdigit(str[i + 1]))) {
					num += str[i];
					nums.push_back(atof(num.c_str()));
					num = "";
				}
				else {
					num += str[i];
				}
				break;
			case '+': case '-': case '*': case '/':
				opers.push_back(str[i]);
			}
		}
	}
	// ���������� ������ ��������� ��������� � �������, �������� � ������� opers.
	// ����� ���������� � ������� start.
	// ���� ��������� ��������� � ������� �� �������, ������� ����� -1.
	int find_multi_division(std::vector<char> opers, int start) {
		for (unsigned int i = start; i < opers.size(); i++) {
			if (opers[i] == '*' || opers[i] == '/') {
				return i;
			}
		}
		return -1;
	}
	// ���������� ������ ��������� �������� � ���������, �������� � ������� opers.
	// ����� ���������� � ������� start.
	// ���� ��������� �������� � ��������� �� �������, ������� ����� -1.
	int find_sum_diff(std::vector<char> opers, int start) {
		for (unsigned int i = start; i < opers.size(); i++) {
			if (opers[i] == '+' || opers[i] == '-') {
				return i;
			}
		}
		return -1;
	}
	void add_sum(double sum, std::vector<double>& nums, std::vector<int> order) {
		for (unsigned int i = 0; i < order.size(); i++) {
			nums[order[i] * 2 - order[i]] = sum;
			nums[order[i] * 2 + 1 - order[i]] = sum;
		}
		std::cout << "ADD_SUM NUMS: ";
		for (double num : nums) {
			std::cout << num << " ";
		}
		std::cout << std::endl;
		std::cout << "ADD_SUM ORDER: ";
		for (double num : order) {
			std::cout << num << " ";
		}
		std::cout << std::endl;
	}
	double calculate(std::vector<char> opers, std::vector<double>& nums, std::vector<int> order) {
		double sum = 0;
		int index = -1;
		do {
			index = find_multi_division(opers, ++index);
			if (index >= 0) {
				order.push_back(index);
				switch (opers[index]) {
				case '*':
					sum = nums[index * 2 - index] * nums[index * 2 + 1 - index];
					nums[index * 2 - index] = nums[index * 2 + 1 - index] = sum;
					break;
				case '/':
					sum = nums[index * 2 - index] / nums[index * 2 + 1 - index];
					nums[index * 2 - index] = nums[index * 2 + 1 - index] = sum;
					break;
				}
				add_sum(sum, nums, order);
			}
		} while (index != -1);
		do {
			index = find_sum_diff(opers, ++index);
			if (index >= 0) {
				order.push_back(index);
				switch (opers[index]) {
				case '+':
					sum = nums[index * 2 - index] + nums[index * 2 + 1 - index];
					//nums[index * 2 - index] = nums[index * 2 + 1 - index] = sum;
					break;
				case '-':
					sum = nums[index * 2 - index] - nums[index * 2 + 1 - index];
					//nums[index * 2 - index] = nums[index * 2 + 1 - index] = sum;
					break;
				}
				add_sum(sum, nums, order);
			}
		} while (index != -1);
		return nums[nums.size() - 1];
	}
	int main() {
		setlocale(LC_ALL, "russian");

		std::string input;
		std::vector<char> arr_opers;
		std::vector<double> arr_nums;
		std::vector<int> arr_order;
		//std::cin >> input;
		//input = "10*10+20*4+6";	// 186
		//input = "10+10*20/4-6";	// 54
		//input = "5+5*5/5-5";		// 5
		parsing(input, arr_opers, arr_nums);
		for (char op : arr_opers) {
			std::cout << op << " ";
		}
		std::cout << std::endl;
		for (double num : arr_nums) {
			std::cout << num << " ";
		}
		std::cout << std::endl;
		std::cout << calculate(arr_opers, arr_nums, arr_order) << std::endl;
		/*for (double num : arr_nums) {
			std::cout << num << " ";
		}
		std::cout << std::endl; * /
		for (int n : arr_order) {
			std::cout << n << " ";
		}
		std::cout << std::endl;
		return 0;
	}
	*/

	/*************************************************************************************************************************************************/

	/* VERSION 0.1
	int main() {
		setlocale(LC_ALL, "russian");

		double sum = 0, num = 0;
		char oper = '_';

		while (true) {
			std::cin >> sum;
			while (true) {
				std::cin >> oper;
				if (oper == '=') break;
				std::cin >> num;
				switch (oper) {
				case '+':
					sum += num;
					break;
				case '-':
					sum -= num;
					break;
				case '*':
					sum *= num;
					break;
				case '/':
					sum /= num;
					break;
				default:
					std::cout << "������! ������ ��������� ���!" << std::endl;
				}
			}
			std::cout << sum << std::endl;
		}

		return 0;
	}
	*/