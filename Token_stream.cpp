
#include <iostream>
#include "errors.h"
#include "constants.h"
#include "Token_stream.h"

const std::string Token_stream::clear_whitespaces(std::string input) {
	std::string new_input = "";
	for (int i = 0; i < input.size(); i++) {
		if (input[i] != ' ') new_input += input[i];
	}
	return new_input;
}

char letter(std::string input, int index) {
	return isalpha(input[index]);
}

std::string string(std::string input, int index) {
	std::string name = "";
	while (isalpha(input[index])) {
		name += input[index++];
	}
	return name;
}
void number(std::string input, int index) {
	std::string str = string(input, index);
}
void initialization(std::string input, int index=0) {
	number(input, index);
}

/*
	letter=20 bar=40
	letter+40-bar

	INITIALIZATION
		string=number
	NUMBER
		number
	STRING
		string
		letter
	LETTER
		letter
* 
* case 'a': case 'b': case 'c': case 'd': case 'f': case 'g': case 'h': case 'i': case 'j': case 'k': case 'l': case 'm':
		case 'n': case 'o': case 'p': case 'q': case 'r': case 's': case 't': case 'u': case 'v': case 'w': case 'x': case 'y': case 'z':
			switch (input[i]) {
			case VARIABLES:
				vars.show_variables();
				break;
			default:
				//init(input);

if ((i > 0 && i - 1 != 1) && i + 1 < input.size() && input[i + 1] == INITIAL) {
	for (int j = i + 2; j < input.size(); j++) {
		num += input[j];
	}
	vars.add_variable(input[i], atof(num.c_str()));
	this->set_declaration(true);
	return;
}
else if (vars.is_variable(input[i]) != -1) {
	tokens.push_back({ NUMBER, vars.get_variable(input[i]) });
}
else if (i + 1 >= input.size() && vars.is_variable(input[i]) == -1) {
	throw ErrorNoVariable{};
}
else {
	throw ErrorInvalidInput{};
}
break;
			}
			break;
int number(std::string input) {

}
char letter(std::string input) {
	if (isalpha(input)) {

	}
}
void init(std::string input) {
	char let = letter(input);
	double num = number(input);
}
*/

// input - строка выражения.
// Функция распределяет выражение на лексемы.
// Если лексема число, то Token.kind = NUMBER
// Иначе Token.kind будет равен самому оператору.
// Если выражение содержит неопознанную лексему, то функция выбросит исключение ErrorUnknownToken.
void Token_stream::parsing(std::string input) {
	if (isalpha(input[0])) {
		std::cout << "LET'S GO!" << std::endl;
	}
	input = this->clear_whitespaces(input);
	std::string num = "";
	int brackets_left = 0;
	int brackets_right = 0;
	for (unsigned int i = 0; i < input.size(); i++) {
		switch (input[i]) {		
		case '0': case '1': case '2': case '3': case '4': case '5': case '6': case '7': case '8': case '9': case '.': case ',': case EXHIBITOR:
			if ((i == input.size() - 1 || ((!isdigit(input[i + 1]) && input[i] != EXHIBITOR)) && ((input[i + 1] != '.') && (input[i + 1] != EXHIBITOR)))) {
				num += input[i];
				tokens.push_back({ NUMBER, atof(num.c_str()) });
				num = "";
			}
			else {
				if (input[i] == '.') {
					num += ',';
					continue;
				}
				if (input[i] == EXHIBITOR) {
					num += input[i];
					continue;
				}
				num += input[i];
			}
			break;
		case '(':
		case '{':
			brackets_left++;
			tokens.push_back({ input[i], 0 });
			break;
		case ')':
		case '}':
			brackets_right++;
			if (brackets_right > brackets_left) {
				tokens.clear();
				throw ErrorMissingLeftBracket{};
			}
			tokens.push_back({ input[i], 0 });
			break;
		case '+': case '-': case '*': case '/': case '%': case FACTORIAL:
			if (i > 0 && (input[i] == '+' && input[i - 1] == EXHIBITOR)) {
				num += '+';
				continue;
			}
			if (i == 0 && (input[0] != '+' && input[0] != '-')) throw ErrorNoLeftExpression{};
			else tokens.push_back({ input[i], 0 });
			break;
		default:
			throw ErrorUnknownToken{};
		}
	}
}
// Возвращает следующую лексему и увеличивает индекс.
// Если индекс выходит за пределы размера вектора, то возвращает -1 и очищает вектор лексем.
Token Token_stream::get() {
	// Лексемы закончились.
	if (index >= tokens.size()) {
		Token t;
		index = 0;
		t.kind = '#';
		t.value = -1;
		tokens.clear();
		return t;
	}
	return tokens[index++];
}
// Вывод лексем в консоль.
const void Token_stream::show_tokens() {
	for (Token t : tokens) {
		std::cout << t.kind << " = " << t.value << std::endl;
	}
}
