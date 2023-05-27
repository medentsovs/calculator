#include <iostream>
#include <vector>
#include "errors.h"
#include "constants.h"
#include "Token.h"
#include "Token_stream.h"
#include "Variables_stack.h"

extern Variables_stack vs;

/*************************************************************************************************/
// ��������������� �������

// input - �������� ������������� ������.
// ������� ���������� ������ �� �������� �������.
// ���������� ������ ��� ��������.
const std::string Token_stream::clear_whitespaces(std::string input) {
	std::string new_input = "";
	for (int i = 0; i < input.size(); i++) {
		if (input[i] != ' ') 
			new_input += input[i];
	}
	return new_input;
}

// str - ������, �������������� ����� ���������� �������� ���������� ��� �����.
// ���������, ��� �������� ���������� ��������: ���������� ��� ������.
// ���������� ������ 'V', ���� ������ �������� ���������� � 'N', ���� ������ �������� ������.
const char Token_stream::number_or_variable(std::string str) {
	if      (isalpha(str[0])) return VARIABLE;
	else if (isdigit(str[0])) return NUMBER;
}

// ��������� �������� ������.
// ����������� ������� ����� � ������ ������.
// ���� �� ����������� ������� ��� ����������� � ��� ���������� � ������� �������, �� ����������� ����������.
// ���� ������ ������ ������, ��� �����, �� ����������� ����������.
// ��� �� ����������� ����������, ���� �� ������� ����� ���� ������������.
void Token_stream::is_brackets() {
	switch (pars[index]) {
	case '{': case '(':
		if ((pars[index + 1] == ')' || pars[index + 1] == '}') && (tokens.size() > 0 && tokens[tokens.size() - 1].kind == FUNCTIONS))
			throw ErrorNotArgumentFunction{};
		left_break++;
		break;
	case '}': case ')':
		right_break++;
		if (right_break > left_break)
			throw ErrorMissingLeftBracket{};
		if (pars[index + 1] == '=')
			throw ErrorIvalidEqually{};
		break;
	}
}

// ������� ��������� ��� �������� ���� ��������� - ������������� �������� ���������� ��� ������ ���������.
// ���� ������ ��������� �� ��������� == , �� ��������� ������� ��������� ���� ��������.
// ���� ������ ��������� ������ 1, �� ����������� ����������.
// ���� ����� ������ ����� ����� ������� "�����" ��� "���������" ��� "�������", �� ������������� ����������.
// ���� ������� ���������, �� ���������� true, ����� false.
bool Token_stream::is_equally() {
	if (pars[index + 1] == INITIAL) {
		index += 2;
		if (comparison > 1) throw ErrorNotComparison{};
		tokens.push_back(Token{ EQUALLY, 0 });
		return true;
	}
	else if (index == 0)
		throw ErrorInvalidInput{};
	else if (tokens[tokens.size() - 1].kind == NUMBER)
		throw ErrorDigitalNotEqually{};
	else if (tokens[tokens.size() - 1].kind == CONSTANTS)
		throw ErrorConstantInitial{};
	else if (tokens[tokens.size() - 1].kind == FUNCTIONS)
		throw ErrorFunctionInitial{};
	return false;
}

// ����������� ������� ���������� ���������.
// ���� ���������� ��������� ������ ������, �� ����������� ����������.
// ���������� false, ����� ������, ����� ������ ����� ����� "=" � ��������� �� ��� ���� ����� ����� �����.
// �� ���� ���������� ������ �������� �� ��������� ==, �� ���������� true �������� is_equaly.
bool Token_stream::is_comparison() {
	switch (pars[index]) {
	case '<': case '>':
		comparison++;
		if (comparison > 1) throw ErrorNotComparison{};
		return false;
	case INITIAL:
		comparison++;
		return is_equally();
	}
	return false;
}

// ��������������� �������
/*************************************************************************************************/

/*************************************************************************************************/
/* ���������� ����������� ������:
	REPEAT
	  ,
	OPERATORS
	  VARIABLE = DIGIT
	  DIGIT
	  DIGIT!
	  +DIGIT
	  -DIGIT
	  (-DIGIT
	  (+DIGIT
	  DIGIT +- DIGIT
	  DIGIT -+ DIGIT
	  DIGIT +|-|/|*|%|>|<|== DIGIT
	  DIGIT)
	  FUNCTION(
	DIGIT
	  NUMBER
	  VARIABLE = CONSTANT = FUNCTION
	VARIABLE = CONSTANT = FUNCTION
	  letter
	  letter_
	  letter number
	  letter_number
	NUMBER
	  number
	  number.number
	  number.numberE+number
	  number.numberE-number
*/
/*************************************************************************************************/

/*************************************************************************************************/
// ������� ���������� ������

// ��������� � ����� ������ ��������.
// ���� ������ �� �������� ���������, �� �������� ����������.
// ���� ������ �� ����� ���������, �� ������� ������� number().
const void Token_stream::operators() {
	char ch = pars[index];
	if (isspecsymbol(ch)) {
		is_brackets();
		if (is_comparison()) return number();
		tokens.push_back(Token{ ch, 0 });
		index++;
	}
	else if (!isdigit(ch) && !isalpha(ch)) {
		throw ErrorUnknownToken{};
	}
	return number();
}

// ��������� � ����� ������ ����������.
// ���� ���������� �� ���� ����������, �� ��������� �������� ���������� � ���� ���������� �� ��������� 0.
// ��������� ������� ����� � ������ ������ Variables_stream � ��������� ��������� ������� � ����� ������.
const void Token_stream::variable() {
	while (true) {
		char ch = pars[index];
		if (isalpha(ch)) {
			current += ch;
			ch = pars[++index];
			if (!isalpha(ch) && ch == '_' || isdigit(ch)) {
				while (isdigit(ch) || ch == '_' || isalpha(ch)) {
					current += ch;
					ch = pars[++index];
				}
			}
		}
		else if (!isalpha(ch) || index >= pars.size()) {
			if (current != "") {
				if (tokens.size() > 1 && (tokens[tokens.size() - 1].kind == 'N' || tokens[tokens.size() - 1].kind == 'V')) throw ErrorUseNext{};
				if (current.size() == 1)	
					if (ishotkey((*current.c_str()))) throw ErrorConstantInitial{};
				int i = vs.is_variable(current);
				if (i == -1) {
					if (pars[index] != '=') throw ErrorNoVariable{};
					i = buffer.size();
					buffer.push_back(Variables{ current, (double)buffer.size(), INITVAR, "" });
				}
				char type = vs.get_type(current);
				tokens.push_back(Token{ type ? type : INITVAR, (double)i });
				current = "";
				// ���� ���������� ����� ����� �������, �� ��������� ���� ���������.
				if ((pars[index] == '(' || pars[index] == '{') && (tokens[tokens.size() - 1].kind != FUNCTIONS)) {
					tokens.push_back(Token{ '*', 0 });
				}
				else if ((pars[index] == '(') && (tokens[tokens.size() - 1].kind == FUNCTIONS) && vs.get_name(tokens[tokens.size() - 1].value) == "roman") {
					
					if (pars[index + 1] == ')') {
						throw ErrorNotArgumentFunction{};
					}
					while (pars[++index] != ')') {
						if (index + 1 >= pars.size()) {
							vs.reset_roman();
							throw ErrorMissingRightBracket{};
						}
						vs.add_roman(pars[index]);
					}
					tokens.push_back(Token{ '(', 0 });
					tokens.push_back(Token{ ROMAN, 0 });
					tokens.push_back(Token{ ')', 0 });
					index++;
				}
			}
			if (index >= pars.size()) {
				index = 0;
				return;
			}
			return operators();
		}
	}
}

// ��������� � ����� ������ �����. ��������� ���������� ����� � ��������� ������ 
// � �����, �������������� � ���������������� ���������.
void Token_stream::number() {
	while (true) {
		char ch = pars[index];
		if (isdigit(ch)) {
			current += ch;
			ch = pars[++index];
			if (!isdigit(ch) && ch == '.' || ch == EXHIBITOR) {
				if (ch == '.') {
					current += ',';
					index++;
					continue;
				}
				else current += ch;
				if (ch == EXHIBITOR && pars[index + 1] == '+' || pars[index + 1] == '-') {
					current += pars[++index];
					index++;
				}
			}
		}
		else if (!isdigit(ch) || index >= pars.size()) {
			if (current != "") {				
				tokens.push_back(Token{ NUMBER, atof(current.c_str()) });
				current = "";
				// ���� ����� ����� ����� �������, �� ��������� ���� ���������.
				if ((pars[index] == '(' || pars[index] == '{') && (tokens[tokens.size() - 1].kind != FUNCTIONS)) {
					tokens.push_back(Token{ '*', 0 });
				}
			}
			if (index >= pars.size()) {
				index = 0;
				return;
			}
			return variable();
		}
	}
}

// ������� ���������� ������
/*************************************************************************************************/

/*************************************************************************************************/
// ������� ��� ������ � ���������

// input - ������, �������� �������������, ������� ���������� ���������.
// ��������� ������� ������ �������� �������������.
void Token_stream::parsing(std::string input) {
	tokens.clear();
	current = "";
	right_break = left_break = comparison = index = 0;
	pars = clear_whitespaces(input);
	number();
}

// ���������� ��������� � ������ ������� ��� # -1, ���� ������� �����������.
Token Token_stream::get() {
	if (index + 1 > tokens.size()) {
		return Token{ EXIT, -1 };
	}
	return tokens[index++];
}

// ���������� ������ �������� ������ �� ���������� ������.
void Token_stream::putback() {
	index--;
}

// index - ������ ���������� � ������ ����������, ������� ����� �������.
// ���������� ��� ����������, ����������� � ������ ����������, �� � �������.
const std::string Token_stream::get_buffer_name(int index) {
	return buffer[index].name;
}

// ���������� ������� ������ �������.
const int Token_stream::get_index() {
	return index;
}

// ������� ��� ������ � ���������
/*************************************************************************************************/

// ���������� �������.
// ������� ����������� �������.
const void Token_stream::show_tokens() {
	std::cout << "-----------------------------------------------" << std::endl;
	std::cout << "TOKENS: " << std::endl;
	for (Token t : tokens) {
		std::cout << t.kind << " = " << t.value << std::endl;
	}
	std::cout << "-----------------------------------------------" << std::endl;
}