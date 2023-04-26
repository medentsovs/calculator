#pragma once

#include <string>
#include <vector>
/*
 * Ввод пользователя необходимо разложить на лексемы:
 * Литералы с плавающей точкой : 3.14, 0.274e2, 42
 * Операторы, такие как + , -, *, / , %
 * Скобки : (, )
 * Общепринятое решение - хранить каждую лексему в виде пары(вид, значение).
 *  Это реализовано с помощью класса Token.
 * Вид идентифицирует лексему - что она собой представляет : число, оператор или скобку?
 *  Для чисел в качестве значения используется само число.
 */

// Класс реализует парсинг и управление лексемами.
class Token_stream {
private:
	// Индекс лексемы, на которой находятся функции грамматических выражений.
	int index = 0;
	// Лексемы хранятся здесь.
	std::vector<Token> tokens;
public:
	void parsing(std::string input);
	Token get();
	void putback();
	void show_tokens();
};

// input - строка выражения.
// Функция распределяет выражение на лексемы.
// Если лексема число, то Token.kind = 8
// Иначе Token.kind будет равен самому оператору.
// Если выражение содержит неопознанную лексему, то функция выбросит исключение ErrorUnknownToken.
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
	// Первый символ является унарным минусом.
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
// Возвращает индекс на предыдущую позицию.
void Token_stream::putback() {
	index--;
}
// Вывод лексем в консоль.
void Token_stream::show_tokens() {
	for (Token t : tokens) {
		std::cout << t.kind << " = " << t.value << std::endl;
	}
}