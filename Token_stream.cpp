#include <iostream>
#include <string>

#include "errors.h"
#include "constans.h"
#include "Token_stream.h"
#include "Variables_stack.h"
#include "Constants_stack.h"
#include "Functions_stack.h"


const std::string Token_stream::clear_whitespaces(std::string input) {
	std::string new_input = "";
	for (int i = 0; i < input.size(); i++) {
		if (input[i] != ' ') new_input += input[i];
	}
	return new_input;
}

void Token_stream::add_exhibitor() {
	this->number += EXHIBITOR;
	this->exhibitor = false;
}

void Token_stream::this_exhibitor(char sign_current) {
	this->number += sign_current;
	this->exhibitor = true;
}

void Token_stream::add_exhibitor_sign(char sign_current) {
	if (sign_current == '-') this->number += '-';
	else					 this->number += '+';
}

// ��� ����������� ������ ����� � ������� ������ � �������, ������� ����� � ������ ����������� ������.
void Token_stream::add_operator(int index, char sign_first_element, char sign_current) {
	if (index == 0 && (sign_first_element != '+' && sign_first_element != '-')) {
		tokens.clear();
		throw ErrorNoLeftExpression{};
	}
	else tokens.push_back({ sign_current, 0 });
}

// ���� ������� ������ �� ����� 0 � ���������� ������ ����� ��� �����,
// �� ����������� ����������, ��� ��� ����� ������� �� ����� ���� ����� ��� ��� ����������,
// ����� ���� ������ ���� ���� ���������.
// ����������� ������� ����� ������ � �������� ������ � ����� ������.
void Token_stream::start_bracket(char sign_current) {	
	this->brackets_left++;
	this->tokens.push_back({ sign_current, 0 });
}

// ����������� ������� ������ ������.
// ������ ��������, ���� ������ ������ ������, ��� �����, 
// �� ����������� ����������, ��� ��� ����� ������ ���������.
// ���� ��������� ������ �������� ����� ��� �����, �� ����������� ����������,
// ��� ��� ����� ������ ��� ������ ���������� � ������� ������ ������ ���� ���������.
// �������� ������ � ����� ������.
void Token_stream::end_bracket(char sign_current, char sign_plus_one) {
	this->brackets_right++;
	if (this->brackets_right > this->brackets_left) {
		this->tokens.clear();
		throw ErrorMissingLeftBracket{};
	}
	if (isdigit(sign_plus_one) || isalpha(sign_plus_one)) {
		this->tokens.clear();
		throw ErrorInvalidInput{};
	}
	this->tokens.push_back({ sign_current, 0 });
}

// ������� ���������, ���������� �� ���������� � ������� ���������� Variables_stack
//  ���� �� ����������, � ��������� ������ ����� ������� ������������� ����������,
// �� �������� ����� ���������� � ������� ����������, ���� ��������� ������ �� �����
// ������� �������������, �� ������������� ���������� - ����� ���������� �� ����������.
//  ����� ��������� � ����� ������ ���������� � �������� � ������� � ������� ����������.
void Token_stream::add_variable(Variables_stack& vs, Functions_stack fs, char sign_plus_one) {
	int count = 0;
	if (vs.is_variable(this->variable) == -1) {
		if (sign_plus_one == INITIAL) {
			count = vs.get_count();
			vs.add_variable(this->variable, count);
		}
		else {
			this->tokens.clear();
			throw ErrorNoVariable{};
		}
	}
	else {
		count = vs.is_variable(this->variable);
	}
	Token t = { VARIABLE, count };
	this->variable = "";
	this->tokens.push_back(t);
}

// ���� ��������� ������ �������� ������, ����������� ����������,
// ��� ��� ���������� �� ����� ��������� ����.
// ����� ��������� � ����� ������ �������� ����������, ���� ������� ����� ����,
// �� �������� ������������� ���������� �������� ������� get_variable,
// ���� ���������� �� ����������, �� �������� ���������� - ���������� �� ����������.
void Token_stream::add_value_of_variable(Variables_stack& vs, Constants_stack cs, Functions_stack fs, char sign_plus_one) {
	double d = 0;
	if (isdigit(sign_plus_one)) {
		this->tokens.clear();
		throw ErrorNumberVariable{};
	}
	if (!fs.is_function(this->variable) && (sign_plus_one == '(' || sign_plus_one == '{')) {
		this->tokens.clear();
		throw ErrorInvalidInput{};
	}
	if (cs.is_constant(this->variable))
		d = cs.get_constant(this->variable);
	else
		d = vs.get_variable(this->variable);
	this->tokens.push_back({ NUMBER, d });
	this->variable = "";
}

// ���� ��������� ������ �������� ������ � �� �������� �������� ���������� 'e',
// ��� ��������� ������ �������� ������ ������,
// �� ����������� ����������, ��� ��� ��� ���������� �� ����� ���� ����� �� ������
// ��� ����� �� ����� ���� ����� �������.
// ����� ��������� � ����� ������ ��������� ��������.
void Token_stream::add_value_of_number(char sign_current, char sign_plus_one) {
	if ((isalpha(sign_plus_one) && sign_plus_one != EXHIBITOR) || 
		(sign_plus_one == '(' || sign_plus_one == '{')) {
		this->tokens.clear();
		throw ErrorInvalidInput{};
	}
	this->number += sign_current;
	this->tokens.push_back({ NUMBER, atof(this->number.c_str()) });
	this->number = "";
}

void Token_stream::add_function(double index) {
	this->tokens.push_back( {FUNCTIONS, index} );
	this->variable = "";
}

void Token_stream::parsing(std::string input, Variables_stack& vs) {
	Constants_stack cs;
	Functions_stack fs;
	this->brackets_left = this->brackets_right = 0;
	this->variable = this->number = "";
	int comparison = 0;
	input = this->clear_whitespaces(input);

	for (unsigned long int i = 0; i < input.size(); i++) {
		switch (input[i]) {
		case 'A': case 'B': case 'C': case 'D': case 'E': case 'F': case 'G': case 'H': case 'I': case 'J': case 'K': case 'L': case 'M':
		case 'N': case 'O': case 'P': case 'Q': case 'R': case 'S': case 'T': case 'U': case 'V': case 'W': case 'X': case 'Y': case 'Z':
		case 'a': case 'b': case 'c': case 'd': case 'e': case 'f': case 'g': case 'h': case 'i': case 'j': case 'k': case 'l': case 'm':
		case 'n': case 'o': case 'p': case 'q': case 'r': case 's': case 't': case 'u': case 'v': case 'w': case 'x': case 'y': case 'z':
			// ������ �������� �������� ���������� 'e' � � ������ ������ ���������� ������ �����,
			// ������ ��� ���� ����������, � �� ������ � �������� ����������.
			if (input[i] == EXHIBITOR && this->exhibitor) {
				this->add_exhibitor();
				continue;
			}
			this->variable += input[i];
			// ���� ������ �������� ��������� �������� � ����������� ������ ���
			// ��������� ������ �� �������� ������, �� �������� ������ ������������ ���������� � ��������� �� ��� �� �������� ������ ������������ ����������,
			// �� ��������� ���������� � ����� ������.
			if (i == input.size() - 1 || !isalpha(input[i + 1]) && (input[i + 1] == INITIAL && input[i + 2] != INITIAL)) {
				if (i == 0 && (input[i] == SHOWVARS || input[i] == SHOWCONSTS || input[i] == SHOWFUNCS || input[i] == HELP)) {
					this->tokens.clear();
					throw ErrorNotLetter{};
				}
				if ((cs.is_constant(this->variable) || fs.is_function(this->variable)) && input[i + 1] == INITIAL) {
					this->tokens.clear();
					throw ErrorConstantInitial{};
				}
				else if (cs.is_constant(this->variable) && input[i + 1] != INITIAL) {
					this->add_value_of_variable(vs, cs, fs, input[i + 1]);
				}
				else if (fs.is_function(this->variable) && input[i + 1] == '(') {
					this->add_function(fs.get_index(this->variable));
				}
				else {
					this->add_variable(vs, fs, input[i + 1]);
				}
			}
			// ���� ������ �������� ��������� �������� � ����������� ������ ���
			// ��������� ������ �� �������� ������, �� �������� ������ ������������ ���������� � ��������� �� ��� �������� ������ ������������ ����������,
			// �� �������� �������� ���������� � ��������� �������� � ����� ������.
			else if (i == input.size() - 1 || !isalpha(input[i + 1]) && (input[i + 1] == INITIAL && input[i + 2] == INITIAL)) {				
				this->add_value_of_variable(vs, cs, fs, input[i + 1]);
			}
			// ��������� ������ �� �������� ������ � �� �������� ������ ������������ ����������,
			// �� �������� �������� ���������� � ��������� �������� � ����� ������.
			else if (!isalpha(input[i + 1]) && input[i + 1] != INITIAL) {
				this->add_value_of_variable(vs, cs, fs, input[i + 1]);
			}
			break;
		case '0': case '1': case '2': case '3': case '4': case '5': case '6': case '7': case '8': case '9': case '.': case ',':
			if (input[i + 1] == '=' && input[i + 2] != '=') {
				this->tokens.clear();
				throw ErrorInvalidInput{};
			}
			// ���� ������ �������� ��������� �������� � ����������� ������ ���
			// ��������� ������ �� �������� ������ � �� �������� ������ � �� �������� �������� ���������� 'e',
			// �� ��������� ����� � ����� ������.
			if ((i == input.size() - 1 || (!isdigit(input[i + 1]) && ((input[i + 1] != '.') && (input[i + 1] != EXHIBITOR))))) {
				this->add_value_of_number(input[i], input[i + 1]);
			}
			// ���� ��������� ������ �������� �������� ���������� 'e',
			// �� ��������� ������� ������ � �������� ����� � 
			// ���������, ��� ��������� ������ - ��� ������ ���������� 'e', 
			// ��� ����, ����� ��������� ��� � ��������� �������� �������� �����.
			else if (input[i + 1] == EXHIBITOR) {
				this->this_exhibitor(input[i]);
			}
			// ����� ������ ��������� ����� � �������� �����, ��� �����, ���� ��� ������������ �����.
			else {
				if (input[i] == '.') {
					this->number += ',';
					continue;
				}
				this->number += input[i];
			}
			break;
		case '(':
		case '{':
			this->start_bracket(input[i]);
			break;
		case ')':
		case '}':
			this->end_bracket(input[i], input[i + 1]);
			break;
		case '+': case '-': case '*': case '/': case '%': case '>': case '<': case INITIAL: case FACTORIAL:
			if (i > 0 && (input[i - 1] == EXHIBITOR && exhibitor)) {
				this->add_exhibitor_sign(input[i]);
				continue;
			}
			if (input[i] == '>' || input[i] == '<' || input[i] == INITIAL) {
				if (++comparison > 1) {
					this->tokens.clear();
					throw ErrorNotComparison{};
				}
			}
			if (input[i] == INITIAL && input[i + 1] == INITIAL) {
				this->add_operator(i, input[0], EQUALLY);
				i++;
				continue;
			}
			this->add_operator(i, input[0], input[i]);
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
		t.kind = EXIT;
		t.value = -1;
		tokens.clear();
		return t;
	}
	return tokens[index++];
}

// ����� ������ � �������.
const void Token_stream::show_tokens() {
	for (Token t : tokens) {
		std::cout << t.kind << " = " << t.value << std::endl;
	}
}