#include <iostream>
#include <vector>
#include "errors.h"
#include "constants.h"
#include "Token.h"
#include "Token_stream.h"
#include "Variables_stack.h"

extern Variables_stack vs;

/*************************************************************************************************/
// ВСПОМОГАТЕЛЬНЫЕ ФУНКЦИИ

// input - введённая пользователем строка.
// Очищает переданную строку от символов пробела.
// Возвращает строку без пробелов.
const std::string Token_stream::clear_whitespaces(std::string input) {
	std::string new_input = "";
	for (int i = 0; i < input.size(); i++) {
		if (input[i] != ' ') 
			new_input += input[i];
	}
	return new_input;
}

// str - строка, представляющую собой накопитель символов переменной или числа.
// Указывает, чем является накопитель символов: переменной или числом.
// Возвращает символ 'V', если строка является переменной и 'N', если строка является числом.
const char Token_stream::number_or_variable(std::string str) {
	if      (isalpha(str[0])) return VARIABLE;
	else if (isdigit(str[0])) return NUMBER;
}

// Обработка символов скобки.
// Увеличивает счётчик левых и правых скобок.
// Если за открывающей скобкой идёт закрывающая и это происходит в лексеме функции, то выбрасывает исключение.
// Если правых скобок больше, чем левых, то выбрасывает исключение.
// Так же выбрасывает исключение, если за скобкой стоит знак присваивания.
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

// Функция проверяет чем является знак равенства - присваиванием значения переменной или знаком сравнения.
// Если знаком сравнения на равенство == , то добавляет лексему сравнения двух значений.
// Если знаков сравнения больше 1, то выбрасывает исключение.
// Если перед знаком равно стоит лексема "Число" или "Константа" или "Функция", то выбрасывается исключение.
// Если лексема добавлена, то возвращает true, иначе false.
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

// Увеличивает счётчик операторов сравнения.
// Если операторов сравнения больше одного, то выбрасывает исключение.
// Возвращает false, кроме случая, когда символ равен знаку "=" и следующий за ним тоже равен этому знаку.
// То есть получается символ проверки на равенство ==, то возвращает true функцией is_equaly.
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

// ВСПОМОГАТЕЛЬНЫЕ ФУНКЦИИ
/*************************************************************************************************/

/*************************************************************************************************/
/* ГРАММАТИКА ОПРЕДЕЛЕНИЯ ЛЕКСЕМ:
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
// ФУНКЦИИ ГРАММАТИКИ ЛЕКСЕМ

// Добавляет в поток лексем оператор.
// Если символ не известна программе, то выбросит исключение.
// Если символ не равен оператору, то вызовет функцию number().
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

// Добавляет в поток лексем переменную.
// Если переменная не была определена, то добавляет название переменной в стек переменных со значением 0.
// Добавляет Римские цифры в объект класса Variables_stream и корректно считывает лексему в поток лексем.
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
				// Если переменная стоит перед скобкой, то добавляем знак умножения.
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

// Добавляет в поток лексем число. Правильно определяет числа с плавающей точкой 
// и числа, представленные в экспоненциальном выражении.
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
				// Если число стоит перед скобкой, то добавляем знак умножения.
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

// ФУНКЦИИ ГРАММАТИКИ ЛЕКСЕМ
/*************************************************************************************************/

/*************************************************************************************************/
// ФУНКЦИИ ДЛЯ РАБОТЫ С ЛЕКСЕМАМИ

// input - строка, введённая пользователем, которую необходимо разобрать.
// Запускает парсинг строки введённой пользователем.
void Token_stream::parsing(std::string input) {
	tokens.clear();
	current = "";
	right_break = left_break = comparison = index = 0;
	pars = clear_whitespaces(input);
	number();
}

// Возвращает следующую в потоке лексему или # -1, если лексемы закончились.
Token Token_stream::get() {
	if (index + 1 > tokens.size()) {
		return Token{ EXIT, -1 };
	}
	return tokens[index++];
}

// Возвращает индекс счётчика лексем на предыдущий символ.
void Token_stream::putback() {
	index--;
}

// index - индекс переменной в буфере переменных, которую нужно вернуть.
// Возвращает имя переменной, находящейся в буфере переменных, по её индексу.
const std::string Token_stream::get_buffer_name(int index) {
	return buffer[index].name;
}

// Возвращает текущий индекс лексемы.
const int Token_stream::get_index() {
	return index;
}

// ФУНКЦИИ ДЛЯ РАБОТЫ С ЛЕКСЕМАМИ
/*************************************************************************************************/

// Отладочная функция.
// Выводит определённые лексемы.
const void Token_stream::show_tokens() {
	std::cout << "-----------------------------------------------" << std::endl;
	std::cout << "TOKENS: " << std::endl;
	for (Token t : tokens) {
		std::cout << t.kind << " = " << t.value << std::endl;
	}
	std::cout << "-----------------------------------------------" << std::endl;
}