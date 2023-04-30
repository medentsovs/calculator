#include <iostream>
#include "Token_stream.h"
#include "errors.h"

const std::string Token_stream::clear_whitespaces(std::string input) {
	std::string new_input = "";
	for (int i = 0; i < input.size(); i++) {
		if (input[i] != ' ') new_input += input[i];
	}
	return new_input;
}
// input - строка выражения.
// Функция распределяет выражение на лексемы.
// Если лексема число, то Token.kind = 8
// Иначе Token.kind будет равен самому оператору.
// Если выражение содержит неопознанную лексему, то функция выбросит исключение ErrorUnknownToken.
void Token_stream::parsing(std::string input) {
	input = this->clear_whitespaces(input);
	std::string num = "";
	int brackets_left = 0;
	int brackets_right = 0;
	for (unsigned int i = 0; i < input.size(); i++) {
		switch (input[i]) {
		case '0': case '1': case '2': case '3': case '4': case '5': case '6': case '7': case '8': case '9': case '.': case ',': case 'e':
			if ((i == input.size() - 1 || ((!isdigit(input[i + 1]) && input[i] != 'e')) && ((input[i + 1] != '.') && (input[i + 1] != 'e')))) {
				num += input[i];
				tokens.push_back({ '8', atof(num.c_str()) });
				num = "";
			}
			else {
				if (input[i] == '.') {
					num += ',';
					continue;
				}
				if (input[i] == 'e') {
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
		case '+': case '-': case '*': case '/': case '%': case '!':
			if (i > 0 && (input[i] == '+' && input[i - 1] == 'e')) {
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