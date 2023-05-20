#pragma once
//#include <vector>
//#include "Token.h"
//#include "Variables.h"
#include "Variables_stack.h"
//#include "Constants_stack.h"
//#include "Functions_stack.h"

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



bool isspecsymbol(char ch);
bool ishotkey(char ch);

class Token_stream {
	std::vector<Token> tokens;		// ����� �������� �������.
	std::vector<Variables> buffer;	// ����� �������� �������� ����������.
	std::string current = "";		// ���������� ��������.
	std::string pars;				// ������, �������� ������������� ��� �������� ������.
	int left_break = 0;				// ������� ����� ������.
	int right_break = 0;			// ������� ������ ������.
	int comparison = 0;				// ������� ���������� ���������.
	int index = 0;					// ������ ������ pars.

	// ������� ���������� ������ �� �������� �������.
	const std::string clear_whitespaces(std::string input) {
		std::string new_input = "";
		for (int i = 0; i < input.size(); i++) {
			if (input[i] != ' ') new_input += input[i];
		}
		return new_input;
	}

	// ���������, ��� �������� ���������� ��������: ���������� ��� ������.
	const char number_or_variable(std::string str) {
		if      (isalpha(str[0])) return VARIABLE;
		else if (isdigit(str[0])) return NUMBER;
	}
	// ��������� �������� ������.
	// ����������� ������� ����� � ������ ������.
	// ���� ������ ������ ������, ��� �����, �� ����������� ����������.
	void is_brackets() {
		switch (pars[index]) {
		case '{': case '(':			
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
	// ���� ����� ������ ����� ����� ������� "�����", �� ������������� ����������.
	bool is_equally() {
		if (pars[index + 1] == INITIAL) {
			index += 2;
			if (comparison > 1) throw ErrorNotComparison{};
			tokens.push_back(Token{ EQUALLY, 0 });
			return true;
		}
		else if (index == 0)
			throw ErrorInvalidInput{};
		else if(tokens[tokens.size() - 1].kind == NUMBER)
			throw ErrorDigitalNotEqually{};
		else if (tokens[tokens.size() - 1].kind == CONSTANTS)
			throw ErrorConstantInitial{};
		else if (tokens[tokens.size() - 1].kind == FUNCTIONS)
			throw ErrorFunctionInitial{};
		return false;
	}
	// ����������� ������� ���������� ���������.
	// ���� ���������� ��������� ������ ������, �� ����������� ����������.
	bool is_comparison() {
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
	// ��������� � ����� ������ ��������.
	const void operators() {
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
	const void variable() {
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
					if(current.size() == 1)	if (ishotkey((*current.c_str()))) throw ErrorConstantInitial{};
					int i = vs.is_variable(current);
					if (i == -1) {
						if (pars[index] != '=') throw ErrorNoVariable{};
						i = buffer.size();
						buffer.push_back(Variables{current, (double)buffer.size(), INITVAR, ""});
					}
					char type = vs.get_type(current);
					tokens.push_back(Token{ type ? type : INITVAR, (double)i});
					current = "";
					if (pars[index] == '(' || pars[index] == '{') {
						tokens.push_back(Token{ '*', 0 });
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
	void number() {
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
			else if(!isdigit(ch) || index >= pars.size()) {
				if (current != "") {
					if (pars[index] == '(' || pars[index] == '{') {
						throw ErrorInvalidInput{};
					}
					tokens.push_back(Token{ NUMBER, atof(current.c_str()) });
					current = "";
				}
				if (index >= pars.size()) {
					index = 0;
					return;
				}
				return variable();
			}
		}
	}	
public:
	// ��������� ������� ������ �������� �������������.
	void parsing(std::string input) {
		tokens.clear();
		current = "";
		right_break = left_break = comparison = index = 0;
		pars = clear_whitespaces(input);
		number();
	}
	// �������� ��������� � ������ �������.
	Token get() {
		if (index + 1 > tokens.size()) {
			return Token{ EXIT, -1 };
		}
		return tokens[index++];
	}
	void putback() {
		index--;
	}
	const std::string get_buffer_name(int index) {
		return buffer[index].name;
	}


	// ���������� �������.
	// ������� ����������� �������.
	const void show_tokens() {
		std::cout << "-----------------------------------------------" << std::endl;
		std::cout << "TOKENS: " << std::endl;
		for (Token t : tokens) {
			std::cout << t.kind << " = " << t.value << std::endl;
		}
		std::cout << "-----------------------------------------------" << std::endl;
	}

	const int get_index() {
		return index;
	}



	/*
	void parsing(std::string input) {
		tokens.clear();
		std::string str = "";
		for (int i = 0; i < input.size(); i++) {
			if (isdigit(input[i])) {
				str += input[i];
				if (!isdigit(input[i + 1])) {
					if (input[i + 1] == '.') {
						str += ',';
						continue;
					}
					if (isalpha(input[i + 1])) {
						if (input[i + 1] == EXHIBITOR) {
							str += EXHIBITOR;
							if (input[i + 2] == '+' || input[i + 2] == '-') {
								str += input[i + 2];
								i++;
							}
							i++;
							continue;
						}
						throw ErrorInvalidInput{};
					}
					if (isalpha(str[0]) && isdigit(str[str.size() - 1])) {
						tokens.push_back(Token{ VARIABLE, 1 });
					}
					else {
						tokens.push_back(Token{ NUMBER, atof(str.c_str()) });
					}
					str = "";
				}
			}
			else if (isalpha(input[i]) || input[i] == '_') {
				str += input[i];
				if (!isalpha(input[i + 1]) || input[i + 1] != '_' && isdigit(input[i + 1])) {
					continue;
				}
				else if (!isalpha(input[i + 1]) || input[i + 1] != '_' && isspecsymbol(input[i + 1])) {
					tokens.push_back(Token{ VARIABLE, 1 });
				}
			}
			else if (isspecsymbol(input[i])) {
				char ch;
				if (input[i] == INITIAL && input[i + 1] == INITIAL) {
					ch = '~';
					i++;
				}
				else {
					ch = input[i];
				}
				tokens.push_back(Token{ ch, 0 });
			}
			else {

			}
		}
	}

	*/

	





	/*
	int index = 0; // ������ �������, �� ������� ��������� ������� �������������� ���������.
	std::vector<Token> tokens;

	std::string number   = "";
	std::string variable = "";
	int brackets_left    = 0;
	int brackets_right   = 0;
	bool exhibitor       = false;
public:
	const std::string clear_whitespaces(std::string input);
	void parsing(std::string input, Variables_stack& vs);
	Token get();
	// ���������� ������ �� ���������� �������.
	void putback() { index--; }
	void index_reset() { index = 0; }
	const void show_tokens();

	// �������, ������������ �������� parsing().
	void add_exhibitor();
	void this_exhibitor(char sign_current);
	void add_exhibitor_sign(char sign_current);
	void add_variable(Variables_stack& vs, Functions_stack fs, char sign_plus_one);
	void add_value_of_variable(Variables_stack& vs, Constants_stack cs, Functions_stack fs, char sign_plus_one);
	void add_value_of_number(char sign_current, char sign_plus_one);
	void start_bracket(char sign_current);
	void end_bracket(char sign_current, char sign_plus_one);
	void add_operator(int index, char sign_first_element, char sign_current);
	void add_function(double index);



	*/
};













/*
#include <string>
#include "Variables_stream.h"
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