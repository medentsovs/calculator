#pragma once
#include <vector>
#include "Token.h"
#include "Variables.h"
#include "Variables_stack.h"

class Token_stream {	
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
	void add_variable(Variables_stack& vs, char sign_plus_one);
	void add_value_of_variable(Variables_stack& vs, char sign_plus_one);
	void add_value_of_number(char sign_current, char sign_plus_one);
	void start_bracket(char sign_current);
	void end_bracket(char sign_current, char sign_plus_one);
	void add_operator(int index, char sign_first_element, char sign_current);
};













/*
#include <string>
#include "Variables_stream.h"
*/
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