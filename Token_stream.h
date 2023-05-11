#pragma once
#include <vector>
#include "Token.h"
#include "Variables.h"
#include "Variables_stack.h"

class Token_stream {
	// Индекс лексемы, на которой находятся функции грамматических выражений.
	int index = 0;
	// Лексемы хранятся здесь.
	std::vector<Token> tokens;
	std::vector<Variables> vars;
public:
	const std::string clear_whitespaces(std::string input);
	void parsing(std::string input, Variables_stack& vs);
	Token get();
	// Возвращает индекс на предыдущую позицию.
	void putback() { index--; }
	const void show_tokens();
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