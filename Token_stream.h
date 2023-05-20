#pragma once
//#include <vector>
//#include "Token.h"
//#include "Variables.h"
#include "Variables_stack.h"
//#include "Constants_stack.h"
//#include "Functions_stack.h"

/*
 * Ввод пользователя необходимо разложить на лексемы:
 * Литералы с плавающей точкой: 3.14, 0.274e2, 42
 * Операторы, такие как + , -, *, / , %, <, >, =, ==
 * Скобки: (, ), {, }
 * Переменные: var, Result, Variable
 * Константы: PI
 * Функции: sqrt, !factorial
 * Общепринятое решение - хранить каждую лексему в виде пары(вид, значение).
 *  Это реализовано с помощью класса Token.
 * Вид идентифицирует лексему - что она собой представляет: число, оператор или скобку?
 *  Для чисел в качестве значения используется само число.
 *  Для переменных, констант и функций - индекс, по которому они находятся в массиве.
 */



bool isspecsymbol(char ch);
bool ishotkey(char ch);

class Token_stream {
	std::vector<Token> tokens;		// Здесь хранятся лексемы.
	std::vector<Variables> buffer;	// Буфер хранения названий переменной.
	std::string current = "";		// Накопитель символов.
	std::string pars;				// Строка, введённая пользователем для удобства работы.
	int left_break = 0;				// Счётчик левых скобок.
	int right_break = 0;			// Счётчик правых скобок.
	int comparison = 0;				// Счётчик операторов сравнения.
	int index = 0;					// Индекс строки pars.

	// Очищает переданную строку от символов пробела.
	const std::string clear_whitespaces(std::string input) {
		std::string new_input = "";
		for (int i = 0; i < input.size(); i++) {
			if (input[i] != ' ') new_input += input[i];
		}
		return new_input;
	}

	// Указывает, чем является накопитель символов: переменной или числом.
	const char number_or_variable(std::string str) {
		if      (isalpha(str[0])) return VARIABLE;
		else if (isdigit(str[0])) return NUMBER;
	}
	// Обработка символов скобки.
	// Увеличивает счётчик левых и правых скобок.
	// Если правых скобок больше, чем левых, то выбрасывает исключение.
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
	// Функция проверяет чем является знак равенства - присваиванием значения переменной или знаком сравнения.
	// Если знаком сравнения на равенство == , то добавляет лексему сравнения двух значений.
	// Если знаков сравнения больше 1, то выбрасывает исключение.
	// Если перед знаком равно стоит лексема "Число", то выбрасывается исключение.
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
	// Увеличивает счётчик операторов сравнения.
	// Если операторов сравнения больше одного, то выбрасывает исключение.
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
	// Добавляет в поток лексем оператор.
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
	// Добавляет в поток лексем переменную.
	// Если переменная не была определена, то добавляет название переменной в стек переменных со значением 0.
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
	// Добавляет в поток лексем число. Правильно определяет числа с плавающей точкой 
	// и числа, представленные в экспоненциальном выражении.
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
	// Запускает парсинг строки введённой пользователем.
	void parsing(std::string input) {
		tokens.clear();
		current = "";
		right_break = left_break = comparison = index = 0;
		pars = clear_whitespaces(input);
		number();
	}
	// Получить следующую в потоке лексему.
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


	// Отладочная функция.
	// Выводит определённые лексемы.
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
	int index = 0; // Индекс лексемы, на которой находятся функции грамматических выражений.
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
	// Возвращает индекс на предыдущую позицию.
	void putback() { index--; }
	void index_reset() { index = 0; }
	const void show_tokens();

	// Функции, используемые функцией parsing().
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
// Класс реализует парсинг и управление лексемами.
class Token_stream {
private:
	
	
	// Переменные.
	Variables_stream vars;
	// Индикатор объявления переменной.
	bool declaration = false;
public:
	
	
	
	


	const bool get_declaration() { return declaration; }
	void set_declaration(bool declaration) { this->declaration = declaration; }
	const Variables_stream get_variable() { return vars; }
};
*/