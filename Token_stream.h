#pragma once
#include <iostream>
#include <vector>
#include "errors.h"
#include "constants.h"
#include "Token.h"
#include "Variables_stack.h"

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

extern bool isspecsymbol(char ch);
extern bool ishotkey(char ch);

class Token_stream {
	std::vector<Token> tokens;		// Здесь хранятся лексемы.
	std::vector<Variables> buffer;	// Буфер хранения названий пользовательских переменных, которые ещё не инициализированы.
	std::string current = "";		// Накопитель символов.
	std::string pars;				// Строка, введённая пользователем для удобства работы.
	int left_break = 0;				// Счётчик левых скобок.
	int right_break = 0;			// Счётчик правых скобок.
	int comparison = 0;				// Счётчик операторов сравнения.
	int index = 0;					// Индекс строки pars.

	const std::string clear_whitespaces(std::string input);
	const char number_or_variable(std::string str);
	void is_brackets();
	bool is_equally();
	bool is_comparison();
	const void operators();
	const void variable();
	void number();

public:
	void parsing(std::string input);
	Token get();
	void putback();
	const std::string get_buffer_name(int index);
	const void show_tokens();
	const int get_index();
};

