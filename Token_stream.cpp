#include <iostream>
#include <string>

#include "errors.h"
#include "constans.h"
#include "Token_stream.h"
#include "Variables_stack.h"


const std::string Token_stream::clear_whitespaces(std::string input) {
	std::string new_input = "";
	for (int i = 0; i < input.size(); i++) {
		if (input[i] != ' ') new_input += input[i];
	}
	return new_input;
}



void Token_stream::parsing(std::string input, Variables_stack& vs) {
	input = this->clear_whitespaces(input);

	std::string num = "";
	std::string var = "";
	int brackets_left = 0;
	int brackets_right = 0;
	bool exhibitor = false;

	for (unsigned long int i = 0; i < input.size(); i++) {
		switch (input[i]) {
		case 'a': case 'b': case 'c': case 'd': case 'e': case 'f': case 'g': case 'h': case 'i': case 'j': case 'k': case 'l': case 'm':
		case 'n': case 'o': case 'p': case 'q': case 'r': case 's': case 't': case 'u': case 'v': case 'w': case 'x': case 'y': case 'z':
			if (input[i] == EXHIBITOR && exhibitor) {
				num += input[i];
				exhibitor = false;
				continue;
			}
			var += input[i];
			if (i == input.size() - 1 || !isalpha(input[i + 1]) && input[i + 1] == INITIAL) {
				int count = 0;
				if (vs.is_variable(var) == -1) {
					count = vs.get_count();
					if(input[i + 1] == INITIAL) 
						vs.add_variable(var, count);
					else {
						tokens.clear();
						throw ErrorNoVariable{};
					}
				}
				else {
					count = vs.is_variable(var);
				}
				Token t = { VARIABLE, count };
				var = "";
				tokens.push_back(t);
			}
			else if (!isalpha(input[i + 1]) && input[i + 1] != INITIAL) {
				if (isdigit(input[i + 1])) {
					tokens.clear();
					throw ErrorNumberVariable{};
				}
				tokens.push_back({ NUMBER, vs.get_variable(var) });
				var = "";
			}
			break;
		case '0': case '1': case '2': case '3': case '4': case '5': case '6': case '7': case '8': case '9': case '.': case ',':
			if ((i == input.size() - 1 || (!isdigit(input[i + 1]) && ((input[i + 1] != '.') && (input[i + 1] != EXHIBITOR))))) {
				if (isalpha(input[i + 1]) && input[i + 1] != EXHIBITOR) {
					tokens.clear();
					throw ErrorInvalidInput{};
				}
				num += input[i];
				tokens.push_back({ NUMBER, atof(num.c_str()) });
				num = "";
			}
			else if (input[i + 1] == EXHIBITOR) {
				exhibitor = true;
				num += input[i];
				continue;
			}
			else {
				if (input[i] == '.') {
					num += ',';
					continue;
				}
				num += input[i];
			}
			break;
		case '(':
		case '{':
			if(i > 0 && (isdigit(input[i - 1]) || isalpha(input[i - 1]))) {
				tokens.clear();
				throw ErrorInvalidInput{};
			}
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
			if (isdigit(input[i + 1]) || isalpha(input[i + 1])) {
				tokens.clear();
				throw ErrorInvalidInput{};
			}
			tokens.push_back({ input[i], 0 });
			break;
		case '+': case '-': case '*': case '/': case '%': case INITIAL: case FACTORIAL:
			if (i > 0 && (input[i - 1] == EXHIBITOR && exhibitor)) {
				if (input[i] == '-') num += '-';
				else num += '+';
				continue;
			}
			if (i == 0 && (input[0] != '+' && input[0] != '-')) {
				tokens.clear();
				throw ErrorNoLeftExpression{};
			}
			else tokens.push_back({ input[i], 0 });
			break;
		default:
			throw ErrorUnknownToken{};
		}
	}
}

// ¬озвращает следующую лексему и увеличивает индекс.
// ≈сли индекс выходит за пределы размера вектора, то возвращает -1 и очищает вектор лексем.
Token Token_stream::get() {
	// Ћексемы закончились.
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

// ¬ывод лексем в консоль.
const void Token_stream::show_tokens() {
	for (Token t : tokens) {
		std::cout << t.kind << " = " << t.value << std::endl;
	}
}