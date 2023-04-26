/*************************************************************************************************************************************************/

/* VERSION 0.6.1
#include <iostream>
#include <string>
#include <vector>
#include <locale.h>

#include "errors.h"
#include "Token.h"
#include "Token_stream.h"

/*
 * Calculator
 * Калькулятор, который способен вычислять сложные выражения.
 * Автор Сергей Меденцов 25.04.2023
 * version 0.6.1
 *
 * Какие фрагменты кода могут вызвать сложности.
 *  ???
 *
 * Основные идеи проекта.
 *  Написать простой, но эффективный калькулятор, который может решать сложные примеры,
 *   например такие: 2+3.1*4=14.4, 10*(20+30)-100=400, 1+2*3/4%5+(6-7*(8))=-47.5
 *
 * Как организован код.
 *  Программа ожидает ввода выражения, затем парсит введённую строку, разбивая её на лексемы,
 *   а затем, считывая лексемы вычисляет выражения и выводит результат выражения.
 *  Лексемы представлены классом Token и содержат вид лексемы и её значение.
 *  Работа с лексемами ложится на класс Token_stream, который парсит строку введёного выражения.
 *   Более подробно работа класса описана в заголовочном файле класса.
 *  Функции expression(), term(), primary() реализуют грамматику выражений, описанную ниже,
 *   и собственно вычисляют выражение.
 *  Ошибки описаны в заголовочном файле errors.
 *   При выбрасывании исключения программа выводит сообщение в консоль и продолжает работу.
 *
 * Какие сделаны предположения о вводных данных.
 *  Вводные данные должны быть одной строкой, содержащей цифры и знаки операторов: + - / * % ( ) . ,
 *   Программа парсит введённую строку и разбивает её на части.
 *
 * Каких фрагментов кода пока не хватает и какие варианты ещё не обработаны.
 *  Проблема с числами меньше ноля! Программа пока не умеет их решать.
 *  Также не реализовано чтение лексемы в экспоненциальном представлении.
 *  Интересная идея добавить в калькулятор переменные m = 9; v = 7; v * m;
 *  А так же тогда уж можно добавить и вычисление математических функций.
 *  Ещё можно выводить циклы!
* /
/*
* РАЗМЫШЛЕНИЯ НАД ЗАДАЧЕЙ:
* - Краткое изложение идеи, лежащей в основе решения.
* - Найти изъяны в своей первой идее.
* - Обсудить задачу и способы её решения с друзьями.
* /

double expression();
double term();
double primary();

Token_stream ts;

int main() {
	setlocale(LC_ALL, "Russian");

	std::string input;

	while (true) {
		try {
			std::cout << "Выражение: ";
			getline(std::cin, input);
			if (!std::cin) throw ErrorInvalidInput{};
			ts.parsing(input);
			std::cout << "Ответ: " << expression() << std::endl;
		}
		catch (ErrorUnknownToken) {
			std::cerr << "ОШИБКА! Неизвестная лексема!\n";
		}
		catch (ErrorDivisionZero) {
			std::cerr << "ОШИБКА! Деление на ноль!\n";
		}
		catch (ErrorInvalidInput) {
			std::cerr << "ОШИБКА! Неверный ввод выражения!\n";
		}
	}

	return 0;
}

/*********************************************************************
*	ГРАММАТИКА ВЫРАЖЕНИЙ:
* --------------------------------------------------------------------
* Выражение:
*	Терм
*	Выражение "+" Терм	// Сложение
*	Выражение "-" Терм	// Вычитание
* Терм:
*	Первичное_выражение
*	Терм "*" Первичное_выражение	// Умножение
*	Терм "/" Первичное_выражение	// Деление
*	Терм "%" Первичное_выражение	// Остаток (деление по модулю)
* Первичное_выражение:
*	Число
*	"(" Выражение ")"	// Группировка
* Число:
*	Литерал_с_плавающей_точкой
********************************************************************** /

/******************************************** /
/* ФУНКЦИИ, РЕАЛИЗУЮЩИЕ ГРАММАТИКУ ВЫРАЖЕНИЙ* /
// Функция проверяет, является ли лексема выражением.
// И работает с операторами '+' и '-'.
double expression() {
	double left = term();
	Token t;
	while (true) {
		t = ts.get();
		switch (t.kind) {
		case '+':
			left += term();
			break;
		case '-':
			left -= term();
			break;
		default:
			return left;
		}
	}
}
// Функция проверяет, является ли лексема термом.
// И работает с операторами '*', '/', '%'
double term() {
	double left = primary();
	Token t;
	while (true) {
		t = ts.get();
		switch (t.kind) {
		case '*':
			left *= primary();
			break;
		case '/':
		{
			double p = primary();
			if (p == 0) {
				throw ErrorDivisionZero{};
			}
			left /= p;
			break;
		}
		case '%':
		{
			double p = primary();
			double res = (int)left / (int)p;
			left = left - (res * p);
			break;
		}
		default:
			ts.putback();	// Возвращаемся на предыдущую лексему
			return left;
		}
	}
}
// Функция проверяет, является ли лексема первичным выражением.
double primary() {
	Token t = ts.get();
	switch (t.kind) {
	case '8':	// Число
		return t.value;
	case '|':	// Отрицательное число
		return 0 - t.value;
	case '(':	// Выражение в скобках
	{
		return expression();
	}
	default:	// Возвращаемся на предыдущую лексему
		ts.putback();
	}
}

/* ФУНКЦИИ, РЕАЛИЗУЮЩИЕ ГРАММАТИКУ ВЫРАЖЕНИЙ* /
/********************************************/

/*************************************************************************************************************************************************/

/* VERSION 0.6
 * Calculator
 * Калькулятор, который способен вычислять сложные выражения.
 * Автор Сергей Меденцов 14.04.2023 - 25.04.2023
 * version 0.6
 *
 * Какие фрагменты кода могут вызвать сложности.
 * Основные идеи проекта.
 * Как организован код.
 * Какие сделаны предположения о вводных данных.
 *  Вводные данные должны быть одной строкой, содержащей числа и знаки операторов. Программа парсит введённую строку и разбивает её на части.
 * Каких фрагментов кода пока не хватает и какие варианты ещё не обработаны.
 *  Выражения, в которых встречаются вложенные скобки не работают! Но проблема не только во вложенных скобках! Одинарные иногда тоже дают сбой!
 *   Для решения нужно попробовать перенести функцию get_token() в класс Token!
 *  Интересная идея добавить в калькулятор переменные m = 9; v = 7; v * m;
 *  А так же тогда уж можно добавить и вычисление математических функций.
 *  Ещё можно выводить циклы!
* /
/*
* ЗАДАЧА:
* Написать простой, но эффективный калькулятор, который может решать сложные примеры,
*  например такие: 2+3.1*4=14.4, 10*(20+30)-100=400, 1+2*3/4%5+(6-7*(8))
* /
/*
* РАЗМЫШЛЕНИЯ НАД ЗАДАЧЕЙ:
* Ввод пользователя необходимо разложить на лексемы:
*  Литералы с плавающей точкой: 3.14, 0.274e2, 42
*  Операторы, такие как +, -, *, /, %
*  Скобки: (, )
* Общепринятое решение - хранить каждую лексему в виде пары (вид, значение).
*  Вид идентифицирует лексему - что она собой представляет: число, оператор или скобку?
*  Для чисел в качестве значения используется само число.
*
* - Краткое изложение идеи, лежащей в основе решения.
* - Найти изъяны в своей первой идее.
* - Обсудить задачу и способы её решения с друзьями.
* /
/*
*	ПСЕВДОКОД:
*
* while (not_finished) {
	read_a_line
	calculate		// Выполнение вычислений
	write_result
* }
*
* /
/*
*	ГРАММАТИКА ВЫРАЖЕНИЙ:
* Выражение:
*	Терм
*	Выражение "+" Терм	// Сложение
*	Выражение "-" Терм	// Вычитание
* Терм:
*	Первичное_выражение
*	Терм "*" Первичное_выражение	// Умножение
*	Терм "/" Первичное_выражение	// Деление
*	Терм "%" Первичное_выражение	// Остаток (деление по модулю)
* Первичное_выражение:
*	Число
*	"(" Выражение ")"	// Группировка
* Число:
*	Литерал_с_плавающей_точкой
* /

class ErrorDivisionZero {};
class ErrorUnknownToken {};
class ErrorInvalidInput {};

class Token {
public:
	char kind;
	double value;
};

std::vector<Token> tokens;

// input - выражение, которое необходимо вычислить.
std::vector<Token> parsing_token(std::string input) {
	std::vector<Token> tokens;
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
	return tokens;
}

void show_token(std::vector<Token> tokens);
double expression();
double term();
double primary();

Token get_token(bool negative = false) {
	static int i = 0;
	if (i >= tokens.size()) {
		Token t;
		i = 0;
		t.kind = '#';
		t.value = -1;
		return t;
	}
	if (negative == true)
		return tokens[i--];

	return tokens[i++];
}
double expression() {
	double left = term();
	Token t;
	while (true) {
		t = get_token();
		switch (t.kind) {
		case '+':
			left += term();
			break;
		case '-':
			left -= term();
			break;
		default:
			return left;
		}
	}
}
double term() {
	double left = primary();
	Token t;
	while (true) {
		t = get_token();
		switch (t.kind) {
		case '*':
			left *= primary();
			break;
		case '/':
		{
			double p = primary();
			if (p == 0) {
				throw ErrorDivisionZero{};
			}
			left /= p;
			break;
		}
		case '%':
		{
			double p = primary();
			double res = (int)left / (int)p;
			left = left - (res * p);
			break;
		}
		default:
			get_token(true);
			return left;
		}
	}
}
double primary() {
	Token t = get_token();
	switch (t.kind) {
	case '8':
		return t.value;
	case '(':
	{
		return expression();
	}
	default:
		get_token(true);
	}
}

int main() {
	setlocale(LC_ALL, "Russian");

	std::string input;

	//input = "1+2+3";					// 6
	//input = "1-2-3";					// -4
	//input = "5 + 5 * 10";				// 55
	//input = "10*10+20*4+6";			// 186
	//input = "10+10*20/4-6";			// 54
	//input = "5+5*5/5-5";				// 5
	//input = "2+3.1*4";				// 14.4
	//input = "10*(20+30)-100";			// 400
	//input = "10 + (10 + 10)";			// 30
	//input = "1+2*3/4%5+(6-7*(8))";	// -50
	while (true) {
		try {
			std::cout << "Выражение: ";
			getline(std::cin, input);
			//std::cin >> input;
			if (!std::cin) throw ErrorInvalidInput{};
			tokens.clear();
			tokens = parsing_token(input);

			std::cout << "Ответ: " << expression() << std::endl;
		}
		catch (ErrorUnknownToken) {
			std::cerr << "ОШИБКА! Неизвестная лексема!\n";
		}
		catch (ErrorDivisionZero) {
			std::cerr << "ОШИБКА! Деление на ноль!\n";
		}
		catch (ErrorInvalidInput) {
			std::cerr << "ОШИБКА! Неверный ввод выражения!\n";
		}
	}

	return 0;
}

void show_token(std::vector<Token> tokens) {
	for (Token t : tokens) {
		std::cout << "'" << t.kind << "'\t";
	}
	std::cout << std::endl;
	for (Token t : tokens) {
		if (t.kind != '8') {
			std::cout << '_';
			std::cout << '\t';
			continue;
		}
		std::cout << t.value << '\t';
	}
	std::cout << std::endl;
}
*/

/*************************************************************************************************************************************************/

/* VERSION 0.5
#include <iostream>
#include <vector>
#include <string>
#include <locale.h>

/*
 * Calculator
 * Калькулятор, который способен вычислять сложные выражения.
 * Автор Сергей Меденцов 08.04.2023-14.04.2023
 * version 0.5
 *
 * Какие фрагменты кода могут вызвать сложности.
 * Основные идеи проекта.
 * Как организован код.
 * Какие сделаны предположения о вводных данных.
 *  Вводные данные должны быть одной строкой, содержащей числа и знаки операторов. Программа парсит введённую строку и разбивает её на части.
 * Каких фрагментов кода пока не хватает и какие варианты ещё не обработаны.
 *  Интересная идея добавить в калькулятор переменные m = 9; v = 7; v * m;
 *  А так же тогда уж можно добавить и вычисление математических функций.
 *  Ещё можно выводить циклы!
* /
/*
* ЗАДАЧА:
* Написать простой, но эффективный калькулятор, который может решать сложные примеры,
*  например такие: 2+3.1*4=14.4, 10*(20+30)-100=400, 1+2*3/4%5+(6-7*(8))
* /
/*
* РАЗМЫШЛЕНИЯ НАД ЗАДАЧЕЙ:*
* Ввод пользователя необходимо разложить на лексемы:
*  Литералы с плавающей точкой: 3.14, 0.274e2, 42
*  Операторы, такие как +, -, *, /, %
*  Скобки: (, )
* Общепринятое решение - хранить каждую лексему в виде пары (вид, значение).
*  Вид идентифицирует лексему - что она собой представляет: число, оператор или скобку?
*  Для чисел в качестве значения используется само число.
*
* - Краткое изложение идеи, лежащей в основе решения.
* - Найти изъяны в своей первой идее.
* - Обсудить задачу и способы её решения с друзьями.
* /
/*
*	ПСЕВДОКОД:
*
* while (not_finished) {
	read_a_line
	calculate		// Выполнение вычислений
	write_result
* }
*
* /
/*
*	ГРАММАТИКА ВЫРАЖЕНИЙ:
* Выражение:
*	Терм
*	Выражение "+" Терм	// Сложение
*	Выражение "-" Терм	// Вычитание
* Терм:
*	Первичное_выражение
*	Терм "*" Первичное_выражение	// Умножение
*	Терм "/" Первичное_выражение	// Деление
*	Терм "%" Первичное_выражение	// Остаток (деление по модулю)
* Первичное_выражение:
*	Число
*	"(" Выражение ")"	// Группировка
* Число:
*	Литерал_с_плавающей_точкой
* /
class Error {};
class Token {
public:
	char kind;
	double value;
};
// tokens - вектор лексем, которые необходимо вывести.
// tokens ожидает класс tokens {kind: ..., value: ...}
void show_token(std::vector<Token> tokens) {
	for (Token t : tokens) {
		std::cout << "'" << t.kind << "'\t";
	}
	std::cout << std::endl;
	for (Token t : tokens) {
		if (t.kind != '8') {
			std::cout << '_';
			std::cout << '\t';
			continue;
		}
		std::cout << t.value << '\t';
	}
	std::cout << std::endl;
}
// input - выражение, которое необходимо вычислить.
// tokens - вектор лексем, на которые необходимо разложить выражение.
void parsing_token(std::string input, std::vector<Token>& tokens) {
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
		default:
			throw Error{};
		}
	}
}
int main() {
	setlocale(LC_ALL, "russian");

	std::string input;
	std::vector<Token> tokens;

	input = "10*10+20*4+6";	// 186
	//input = "10+10*20/4-6";	// 54
	//input = "5+5*5/5-5";		// 5
	//input = "2+3.1*4";		// 14.4
	//input = "10*(20+30)-100";	// 400
	input = "1+2*3/4%5+(6-7*(8))";

	parsing_token(input, tokens);

	return 0;
}
// brackets - должен содержать индексы вектора Token, в которых находятся скобки ().
void show_brackets(std::vector<int> brackets) {
	for (int i = 0; i < brackets.size(); i++) {
		std::cout << brackets[i] << " ";
	}
}
// tokens - вектор лексем, содержащихся в выражении.
// index - начальный индекс вектора tokens, с которого необходимо начать смещение.
// end - конечный индекс, которым необходимо закончить смещение.
void shift_token(std::vector<Token>& tokens, int index, int end = -1) {
	if (end == -1) end = tokens.size() - 2;
	for (unsigned int i = index; i < end; i++) {
		tokens[i] = tokens[i + 2];
	}
	tokens.pop_back();
	tokens.pop_back();
}
// tokens - вектор лексем.
double order_execution(std::vector<Token>& tokens, int start = 0, int end = -1) {
	int count = end;
	if (end == -1) {
		count = tokens.size();
	}
	for (int i = start; i < (count - 1); i++) {
		switch (tokens[i].kind) {
		case '*':
			tokens[i - 1].value *= tokens[i + 1].value;
			shift_token(tokens, i);
			count -= 2;
			i -= 2;
			break;
		case '/':
			tokens[i - 1].value /= tokens[i + 1].value;
			shift_token(tokens, i);
			count -= 2;
			i -= 2;
			break;
		}
	}
	for (int i = start; i < (count - 1); i++) {
		switch (tokens[i].kind) {
		case '+':
			tokens[i - 1].value += tokens[i + 1].value;
			shift_token(tokens, i);
			count -= 2;
			i -= 2;
			break;
		case '-':
			tokens[i - 1].value -= tokens[i + 1].value;
			shift_token(tokens, i);
			count -= 2;
			i -= 2;
			break;
		}
	}
	return tokens[0].value;
}
std::vector<int> find_brackets(std::vector<Token> tokens) {
	std::vector<int> indexes;
	for (unsigned int i = 0; i < tokens.size(); i++) {
		if (tokens[i].kind == '(' || tokens[i].kind == ')') {
			indexes.push_back(i);
		}
	}
	return indexes;
}

*/

/*************************************************************************************************************************************************/

	/*
	* VERSION 0.4
	* #include <iostream>
	#include <vector>
	#include <string>
	#include <locale.h>
	/*
	 * Calculator
	 * Калькулятор, который способен вычислять сложные выражения.
	 * Автор Сергей Меденцов 06.04.2023-07.04.2023
	 * version 0.4
	 *
	 * Какие фрагменты кода могут вызвать сложности.
	 * Основные идеи проекта.
	 * Как организован код.
	 * Какие сделаны предположения о вводных данных.
	 *  Вводные данные должны быть одной строкой, содержащей числа и знаки операторов. Программа парсит введённую строку и разбивает её на части.
	 * Каких фрагментов кода пока не хватает и какие варианты ещё не обработаны.
	 *  Интересная идея добавить в калькулятор переменные m = 9; v = 7; v * m;
	 *  А так же тогда уж можно добавить и вычисление математических функций.
	 *  Ещё можно выводить циклы!
	* /
	/*
	* ЗАДАЧА:
	* Написать простой, но эффективный калькулятор, который может решать сложные примеры,
	*  например такие: 2+3.1*4=14.4, 10*(20+30)-100=400, 1+2*3/4%5+(6-7*(8))
	* /
	/*
	* РАЗМЫШЛЕНИЯ НАД ЗАДАЧЕЙ:
	* Проблема со скобками, которую я пока не пытался решать.
	* Ещё нужно подумать, как выявлять ошибки, такие как 2+*3 и 2&3!
	*
	* КОРОЧЕ происходит какая то путаница со скобками. Нужно подумать, как лучше всё реализовать!
	*
	* Ввод пользователя необходимо разложить на лексемы:
	*  Литералы с плавающей точкой: 3.14, 0.274e2, 42
	*  Операторы, такие как +, -, *, /, %
	*  Скобки: (, )
	* Общепринятое решение - хранить каждую лексему в виде пары (вид, значение).
	*  Вид идентифицирует лексему - что она собой представляет: число, оператор или скобку?
	*  Для чисел в качестве значения используется само число.
	*
	* - Краткое изложение идеи, лежащей в основе решения.
	* - Найти изъяны в своей первой идее.
	* - Обсудить задачу и способы её решения с друзьями.
	* /
	/*
	*	ПСЕВДОКОД:
	*
	* while (not_finished) {
		read_a_line
		calculate		// Выполнение вычислений
		write_result
	* }
	*
	* /
	/*
	*	ГРАММАТИКА ВЫРАЖЕНИЙ:
	* Выражение:
	*	Терм
	*	Выражение "+" Терм	// Сложение
	*	Выражение "-" Терм	// Вычитание
	* Терм:
	*	Первичное_выражение
	*	Терм "*" Первичное_выражение	// Умножение
	*	Терм "/" Первичное_выражение	// Деление
	*	Терм "%" Первичное_выражение	// Остаток (деление по модулю)
	* Первичное_выражение:
	*	Число
	*	"(" Выражение ")"	// Группировка
	* Число:
	*	Литерал_с_плавающей_точкой
	* /
	class Error {};
	class Token {
	public:
		char kind;
		double value;
	};
	// input - выражение, которое необходимо вычислить.
	// tokens - вектор лексем, на которые необходимо разложить выражение.
	void parsing_token(std::string input, std::vector<Token>& tokens) {
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
			default:
				throw Error{};
			}
		}
	}
	// tokens - вектор лексем, которые необходимо вывести.
	// tokens ожидает класс tokens {kind: ..., value: ...}
	void show_token(std::vector<Token> tokens) {
		for (Token t : tokens) {
			std::cout << "'" << t.kind << "'\t";
		}
		std::cout << std::endl;
		for (Token t : tokens) {
			if (t.kind != '8') {
				std::cout << '\t';
				continue;
			}
			std::cout << t.value << '\t';
		}
		std::cout << std::endl;
	}
	// brackets - должен содержать индексы вектора Token, в которых находятся скобки ().
	void show_brackets(std::vector<int> brackets) {
		for (int i = 0; i < brackets.size(); i++) {
			std::cout << brackets[i] << " ";
		}
	}
	// tokens - вектор лексем, содержащихся в выражении.
	// index - начальный индекс вектора tokens, с которого необходимо начать смещение.
	// end - конечный индекс, которым необходимо закончить смещение.
	void shift_token(std::vector<Token>& tokens, int index, int end = -1) {
		if (end == -1) end = tokens.size() - 2;
		for (unsigned int i = index; i < end; i++) {
			tokens[i] = tokens[i + 2];
		}
		tokens.pop_back();
		tokens.pop_back();
	}
	// tokens - вектор лексем.
	double order_execution(std::vector<Token>& tokens, int start = 0, int end = -1) {
		int count = end;
		if (end == -1) {
			count = tokens.size();
		}
		for (int i = start; i < (count - 1); i++) {
			switch (tokens[i].kind) {
			case '*':
				tokens[i - 1].value *= tokens[i + 1].value;
				shift_token(tokens, i);
				count -= 2;
				i -= 2;
				break;
			case '/':
				tokens[i - 1].value /= tokens[i + 1].value;
				shift_token(tokens, i);
				count -= 2;
				i -= 2;
				break;
			}
		}
		for (int i = start; i < (count - 1); i++) {
			switch (tokens[i].kind) {
			case '+':
				tokens[i - 1].value += tokens[i + 1].value;
				shift_token(tokens, i);
				count -= 2;
				i -= 2;
				break;
			case '-':
				tokens[i - 1].value -= tokens[i + 1].value;
				shift_token(tokens, i);
				count -= 2;
				i -= 2;
				break;
			}
		}
		return tokens[0].value;
	}
	std::vector<int> find_brackets(std::vector<Token> tokens) {
		std::vector<int> indexes;
		for (unsigned int i = 0; i < tokens.size(); i++) {
			if (tokens[i].kind == '(' || tokens[i].kind == ')') {
				indexes.push_back(i);
			}
		}
		return indexes;
	}
	int main() {
		setlocale(LC_ALL, "russian");
		std::string input;
		std::vector<Token> tokens;
		//while (true) {
			//std::cout << "Выражение: ";
		std::cin >> input;
		//input = "10*10+20*4+6";	// 186
		//input = "10+10*20/4-6";	// 54
		//input = "5+5*5/5-5";		// 5
		//input = "2+3.1*4";		// 14.4
		//input = "10*(20+30)-100";	// 400
		//input = "1+2*3/4%5+(6-7*(8))";
		try {
			parsing_token(input, tokens);
			std::vector<int> brackets = find_brackets(tokens);
			if (brackets.size() != 0) {
				order_execution(tokens, brackets[brackets.size() / 2 - 1], brackets[brackets.size() / 2]);
				show_brackets(brackets);
			}
			//show_token(tokens);
			std::cout << "Ответ: " << order_execution(tokens) << std::endl;
			//tokens.clear();
		}
		catch (Error) {
			std::cerr << "Error!!!\n";
		}
		//}
		return 0;
	}
	*/
	/*************************************************************************************************************************************************/
	/* VERSION 0.3
	 * Calculator
	 * Калькулятор, который способен вычислять сложные выражения.
	 * Автор Сергей Меденцов 04.04.2023 - 05.04.2023
	 * version 0.3
	 *
	 * Какие фрагменты кода могут вызвать сложности.
	 *  ПРОГРАММА НЕ РАБОТАЕТ. ОШИБКА ПЕРЕПОЛНЕНИЯ МАССИВА!
	 * Основные идеи проекта.
	 * Как организован код.
	 * Какие сделаны предположения о вводных данных.
	 *  Вводные данные должны быть одной строкой, содержащей числа и знаки операторов. Программа парсит введённую строку и разбивает её на части.
	 * Каких фрагментов кода пока не хватает и какие варианты ещё не обработаны.
	 *  Функция parsing - можно в дальнейшем сделать проверку на корректность ввода строки с выражением.
	 *  Не могу сделать, чтобы парсер правильно считывал дробное число!
	 *
	 * ПРИДУМАЛ НОВЫЙ АЛГОРИТМ:
	 *  Нам нужно после распределения строки по векторам вычислить очерёдность операторов и поместить их в массив order.
	 *  Допустим получислось 1 2 0 3.
	 *  Производим первое вычисление в ячейках 1 и 1+1, сохраняем результат в ячейке 1, а в ячейку 1+1 переносим файлы из ячейки следующей за 1+1.
	 *  То есть смещаем все числа, а последнюю ячейку удаляем. Также в вектрое order у нас получается другая последовательность. Все ячейки мы уменьшаем на 1:
	 *  То есть теперь в массиве order будет 1 0 2, первую ячейку мы убрали. И так далее, в массиве order следующие значения будут 0 1 и 0.
	 *  А в массиве чисел будет оставаться всё меньше ячеек, пока не останется одна - это и будет наш ответ!
	* /
	/*
	* ЗАДАЧА:
	* Написать простой, но эффективный калькулятор, который может решать сложные примеры,
	*  например такие: 2+3.1*4=14.4, 10*(20+30)-100=400
	* /
	/*
	* РАЗМЫШЛЕНИЯ НАД ЗАДАЧЕЙ:
	*
	* Ввод пользователя необходимо разложить на лексемы:
	*  Литералы с плавающей точкой: 3.14, 0.274e2, 42
	*  Операторы, такие как +, -, *, /, %
	*  Скобки: (, )
	* Общепринятое решение - хранить каждую лексему в виде пары (вид, значение).
	*  Вид идентифицирует лексему - что она собой представляет: число, оператор или скобку?
	*  Для чисел в качестве значения используется само число.
	* (_Как написать придуманный мной алгоритм?_)
	*  Сначала нужно определить в какой последовательности выполняются операторы.
	*   Это можно сделать с помощью функций find_multi_division find_sum_diff
	*  Затем начать поочерёдно выполнять операторы используя вектор с числами.
	*  Число сохранять в первую ячейку, а все следующие ячейки нужно смещать, удаляя вторую использованную.
	*  Доступ к ячейкам при операции по прежнему достигается с помощью формул: [index * 2 - index, index * 2 + 1 - index]
	*
	* ПОЛУЧАЕТСЯ СЛИШКОМ ЗАПУТАННЫЙ КОД ИЗ ЗА ИСПОЛЬЗОВАНИЯ СТОЛЬКИХ НЕСВЯЗАННЫХ ВЕКТОРОВ. НУЖНО КАК_ТО СВЯЗАТЬ ИХ В КУЧУ.
	*
	* - Краткое изложение идеи, лежащей в основе решения.
	* - Найти изъяны в своей первой идее.
	* - Обсудить задачу и способы её решения с друзьями.
	* /
	/*
	*	ПСЕВДОКОД:
	*
		read_a_line
		calculate		// Выполнение вычислений
		write_result
	*
	* /
	// Читает введённую пользователем строку и разбивает её на числа и операторы.
	// Параметр str: это и есть строка введённая пользователем.
	// Параметр opers: ссылка на массив, в который сохраняются операторы.
	// Параметр nums: ссылка на массив, в который сохраняются числа.
	// Проверок функция не делает, хотя нужно было бы.
	void parsing(std::string str, std::vector<char>& opers, std::vector<double>& nums) {
		std::string num = "";
		for (unsigned int i = 0; i < str.size(); i++) {
			switch (str[i]) {
			case '0': case '1': case '2': case '3': case '4': case '5': case '6': case '7': case '8': case '9':
				if ((i == str.size() - 1) || !(isdigit(str[i + 1]))) {
					num += str[i];
					nums.push_back(atof(num.c_str()));
					num = "";
				}
				else {
					num += str[i];
				}
				break;
			case '+': case '-': case '*': case '/':
				opers.push_back(str[i]);
			}
		}
	}
	// Возвращает индекс оператора умножения и деления, наёденные в векторе opers.
	// Поиск начинается с позиции start.
	// Если операторы умножения и деления не найдены, функция вернёт -1.
	int find_multi_division(std::vector<char> opers, int start) {
		for (unsigned int i = start; i < opers.size(); i++) {
			if (opers[i] == '*' || opers[i] == '/') {
				return i;
			}
		}
		return -1;
	}
	// Возвращает индекс оператора сложения и вычитания, наёденные в векторе opers.
	// Поиск начинается с позиции start.
	// Если операторы сложения и вычитания не найдены, функция вернёт -1.
	int find_sum_diff(std::vector<char> opers, int start) {
		for (unsigned int i = start; i < opers.size(); i++) {
			if (opers[i] == '+' || opers[i] == '-') {
				return i;
			}
		}
		return -1;
	}
	void determine_order(std::vector<char> opers, std::vector<int>& order) {
		int index = -1;
		while (true) {
			index = find_multi_division(opers, ++index);
			if (index == -1) break;
			order.push_back(index);
		}
		index = -1;
		while (true) {
			index = find_sum_diff(opers, ++index);
			if (index == -1) break;
			order.push_back(index);
		}
	}
	void shift(std::vector<int>& order, std::vector<double>& nums, int index) {
		for (unsigned int j = order[index]; j < order.size() - 1; j++) {
			std::cout << "ORDER: " << order[index] << std::endl;
			std::cout << j << " - NUMS[j] = " << nums[j] << ", " << j + 1 << " - NUMS[j + 1] = " << nums[j + 1] << std::endl;
			nums[j] = nums[j + 1];
		}
		nums.pop_back();
		/*for (unsigned int j = 0; j < order.size(); j++) {
			--order[j];
			if (order[j] < 0)
				order[j] = 0;
		}* /
	}
	* /
	int main() {
		setlocale(LC_ALL, "russian");

		std::string input;
		std::vector<char> arr_opers;
		std::vector<double> arr_nums;
		std::vector<int> arr_order;

		//std::cin >> input;
		//input = "10*10+20*4+6";	// 186
		input = "10+10*20/4-6";	// 54
		//input = "5+5*5/5-5";		// 5
		//input = "2+3.1*4";		// 14.4
		//input = "10*(20+30)-100";	// 400

		parsing(input, arr_opers, arr_nums);
		determine_order(arr_opers, arr_order);
		for (unsigned int i = 0; i < arr_order.size(); i++) {
			switch (arr_opers[arr_order[i]]) {
			case '+':
				arr_nums[arr_order[i] * 2 - arr_order[i]] += arr_nums[arr_order[i] * 2 + 1 - arr_order[i]];
				shift(arr_order, arr_nums, i);
				break;
			case '-':
				arr_nums[arr_order[i] * 2 - arr_order[i]] -= arr_nums[arr_order[i] * 2 + 1 - arr_order[i]];
				shift(arr_order, arr_nums, i);
				break;
			case '*':
				arr_nums[arr_order[i] * 2 - arr_order[i]] *= arr_nums[arr_order[i] * 2 + 1 - arr_order[i]];
				shift(arr_order, arr_nums, i);
				break;
			case '/':
				arr_nums[arr_order[i] * 2 - arr_order[i]] /= arr_nums[arr_order[i] * 2 + 1 - arr_order[i]];
				shift(arr_order, arr_nums, i);
				break;
			}
		}

		std::cout << "ОТВЕТ: " << arr_nums[0] << std::endl;

		return 0;
	}
	*/

	/*************************************************************************************************************************************************/

	/* VERSION 0.2
	 * Calculator
	 * Калькулятор, который способен вычислять сложные выражения.
	 * Автор Сергей Меденцов 01.04.2023 - 04.04.2023
	 * version 0.2
	 *
	 * Какие фрагменты кода могут вызвать сложности.
	 * Основные идеи проекта.
	 * Как организован код.
	 * Какие сделаны предположения о вводных данных.
	 *  Вводные данные должны быть одной строкой, содержащей числа и знаки операторов. Программа парсит введённую строку и разбивает её на части.
	 * Каких фрагментов кода пока не хватает и какие варианты ещё не обработаны.
	 *  Функция parsing - можно в дальнейшем сделать проверку на корректность ввода строки с выражением.
	 *
	 *  Криво, но реализовано, что программа может подсчитать сначала умножение и деление, а затем сложение и вычитание.
	 *  Проблема в том, что нужен ещё один вектор для хранения индексов уже бывших в употреблении операторов,
	 *  чтобы сохранять результат не только в ближайших ячейках, а во всех использованных ячейках.
	 *  А то пока программа решает выражения не совсем корректно.
	 *   Но и после добавления вектора всё равно возникает проблема. Проблема вот в чём:
	 *    если умножение идёт перед сложением, то всё отрабатывает хорошо, но
	 *    если сначала идёт сложение, а потом умножение, то ответ неверный, потому что в одной из слагаемых ячеек остаётся старое число, но
	 *    если обновлять это число, то не будет работать выражение, в котором умножение идёт первым.
	 *   Как вариант можно попробовать...
	 *  НЕ ЗНАЮ ПОКА ЧТО МОЖНО ПОПРОБОВАТЬ! НУЖНО СНОВА ДУМАТЬ! НЕ СРОАБОТАЛА МОЯ ТЕОРИЯ!!! Хотя была интересная.
	 *
	 * ПРИДУМАЛ НОВЫЙ АЛГОРИТМ:
	 *  Нам нужно после распределения строки по векторам вычислить очерёдность операторов и поместить их в массив order.
	 *  Допустим получислось 1 2 0 3.
	 *  Производим первое вычисление в ячейках 1 и 1+1, сохраняем результат в ячейке 1, а в ячейку 1+1 переносим файлы из ячейки следующей за 1+1.
	 *  То есть смещаем все числа, а последнюю ячейку удаляем. Также в вектрое order у нас получается другая последовательность. Все ячейки мы уменьшаем на 1:
	 *  То есть теперь в массиве order будет 1 0 2, первую ячейку мы убрали. И так далее, в массиве order следующие значения будут 0 1 и 0.
	 *  А в массиве чисел будет оставаться всё меньше ячеек, пока не останется одна - это и будет наш ответ!
	* /
	/ *
	* ЗАДАЧА:
	* Написать простой, но эффективный калькулятор, который может решать сложные примеры,
	*  например такие: 2+3.1*4=14.4, 10*(20+30)-100=400
	* /
	/*
	* РАЗМЫШЛЕНИЯ НАД ЗАДАЧЕЙ:
	* Что должна делать программа?
	*  Программа должна правильно посчитать введённое в консоль выражение.
	* Как бы я хотел взаимодействовать с программой?
	*  Я ввожу выражение в консоль, жму Enter, а программа выдаёт мне результат вычисления.
	* Как написать такую программу?
	*  Загвоздка состоит в том, что при вводе выражения компьютер будет пытаться вычислять её по ходу.
	*  Значит необходимо, чтобы программа начинала вычисления после того, как будет введено всё выражение.
	*  Это означает, что нам нужен парсер введённой строки, который сможет корректно разбить всю строку на составляющие.
	*  (_Как сделать парсер? Что он должен делать?_)
	*   Парсер должен считывать строку посимвольно и помещать цифры в один массив, а операторы в другой массив.
	*   (_Первая загвоздка, как считать цифру целиком, а не по одной цифре?_)
	*    При считывании символов проверять, следующий символ является цифрой или оператором
	*    и если является оператором, то сохранить в ячейку массива.
	*   (_Но как он поймёт какая цифра к какому оператору относится?_)
	*    Каждый оператор имеет две цифры слева и справа от себя. Если закидывать операторы в массив по порядку,
	*    формула для вычисления чисел относящиеся к оператору такая: [index * 2 - index, index * 2 + 1 - index]
	*   Затем нам нужно найти сначала операторы умножения и деления и их аргументы,
	*   подсчитать и сохранить результат в обеих ячейках, относящихся к оператору.
	*   Это позволит нам правильно выполнить сложение и вычитание.
	*   Скорее всего нужно создать ещё один массив, который будет содержать индексы операторов,
	*   которые уже выполнили свою работу, для того чтобы с их помощью сохранить получившееся число в ячейках чисел,
	*   к которым они относятся, для того, чтобы правильно вычислить последующие операторы.
	*   (_Но как в таком случае работать со скобками?_)
	*    Опять же, можно начать вычисление со скобок.
	*    (_Но, если скобки будут вложенными?_)
	*     Значит нужно искать сначала самую вложенную скобку, вычислять все операторы, которые находятся в ней,
	*     а затем вычислять наружные скобки, а затем уже всё остальное.
	*     (_Как всё это реализовать?_)
	*      По ходу нужна рекурсивная функция, которая будет проверять скобки.
	*     Я думаю сначала нужно попробовать написать решение без скобок, а потом подключить и скобки,
	*     так как логика останется та же, просто её нужно грамотно применить в скобках.
	*
	* - Краткое изложение идеи, лежащей в основе решения.
	* - Найти изъяны в своей первой идее.
	* - Обсудить задачу и способы её решения с друзьями.
	* /
	/*
	*	ПСЕВДОКОД:
	*
		read_a_line
		calculate		// Выполнение вычислений
		write_result
	*
	* /
	/*
	// Читает введённую пользователем строку и разбивает её на числа и операторы.
	// Параметр str: это и есть строка введённая пользователем.
	// Параметр opers: ссылка на массив, в который сохраняются операторы.
	// Параметр nums: ссылка на массив, в который сохраняются числа.
	// Проверок функция не делает, хотя нужно было бы.
	void parsing(std::string str, std::vector<char>& opers, std::vector<double>& nums) {
		std::string num = "";
		for (unsigned int i = 0; i < str.size(); i++) {
			switch (str[i]) {
			case '0': case '1': case '2': case '3': case '4': case '5': case '6': case '7': case '8': case '9':
				if ((i == str.size() - 1) || !(isdigit(str[i + 1]))) {
					num += str[i];
					nums.push_back(atof(num.c_str()));
					num = "";
				}
				else {
					num += str[i];
				}
				break;
			case '+': case '-': case '*': case '/':
				opers.push_back(str[i]);
			}
		}
	}
	// Возвращает индекс оператора умножения и деления, наёденные в векторе opers.
	// Поиск начинается с позиции start.
	// Если операторы умножения и деления не найдены, функция вернёт -1.
	int find_multi_division(std::vector<char> opers, int start) {
		for (unsigned int i = start; i < opers.size(); i++) {
			if (opers[i] == '*' || opers[i] == '/') {
				return i;
			}
		}
		return -1;
	}
	// Возвращает индекс оператора сложения и вычитания, наёденные в векторе opers.
	// Поиск начинается с позиции start.
	// Если операторы сложения и вычитания не найдены, функция вернёт -1.
	int find_sum_diff(std::vector<char> opers, int start) {
		for (unsigned int i = start; i < opers.size(); i++) {
			if (opers[i] == '+' || opers[i] == '-') {
				return i;
			}
		}
		return -1;
	}
	void add_sum(double sum, std::vector<double>& nums, std::vector<int> order) {
		for (unsigned int i = 0; i < order.size(); i++) {
			nums[order[i] * 2 - order[i]] = sum;
			nums[order[i] * 2 + 1 - order[i]] = sum;
		}
		std::cout << "ADD_SUM NUMS: ";
		for (double num : nums) {
			std::cout << num << " ";
		}
		std::cout << std::endl;
		std::cout << "ADD_SUM ORDER: ";
		for (double num : order) {
			std::cout << num << " ";
		}
		std::cout << std::endl;
	}
	double calculate(std::vector<char> opers, std::vector<double>& nums, std::vector<int> order) {
		double sum = 0;
		int index = -1;
		do {
			index = find_multi_division(opers, ++index);
			if (index >= 0) {
				order.push_back(index);
				switch (opers[index]) {
				case '*':
					sum = nums[index * 2 - index] * nums[index * 2 + 1 - index];
					nums[index * 2 - index] = nums[index * 2 + 1 - index] = sum;
					break;
				case '/':
					sum = nums[index * 2 - index] / nums[index * 2 + 1 - index];
					nums[index * 2 - index] = nums[index * 2 + 1 - index] = sum;
					break;
				}
				add_sum(sum, nums, order);
			}
		} while (index != -1);
		do {
			index = find_sum_diff(opers, ++index);
			if (index >= 0) {
				order.push_back(index);
				switch (opers[index]) {
				case '+':
					sum = nums[index * 2 - index] + nums[index * 2 + 1 - index];
					//nums[index * 2 - index] = nums[index * 2 + 1 - index] = sum;
					break;
				case '-':
					sum = nums[index * 2 - index] - nums[index * 2 + 1 - index];
					//nums[index * 2 - index] = nums[index * 2 + 1 - index] = sum;
					break;
				}
				add_sum(sum, nums, order);
			}
		} while (index != -1);
		return nums[nums.size() - 1];
	}
	int main() {
		setlocale(LC_ALL, "russian");

		std::string input;
		std::vector<char> arr_opers;
		std::vector<double> arr_nums;
		std::vector<int> arr_order;
		//std::cin >> input;
		//input = "10*10+20*4+6";	// 186
		//input = "10+10*20/4-6";	// 54
		//input = "5+5*5/5-5";		// 5
		parsing(input, arr_opers, arr_nums);
		for (char op : arr_opers) {
			std::cout << op << " ";
		}
		std::cout << std::endl;
		for (double num : arr_nums) {
			std::cout << num << " ";
		}
		std::cout << std::endl;
		std::cout << calculate(arr_opers, arr_nums, arr_order) << std::endl;
		/*for (double num : arr_nums) {
			std::cout << num << " ";
		}
		std::cout << std::endl; * /
		for (int n : arr_order) {
			std::cout << n << " ";
		}
		std::cout << std::endl;
		return 0;
	}
	*/

	/*************************************************************************************************************************************************/

	/* VERSION 0.1
	int main() {
		setlocale(LC_ALL, "russian");

		double sum = 0, num = 0;
		char oper = '_';

		while (true) {
			std::cin >> sum;
			while (true) {
				std::cin >> oper;
				if (oper == '=') break;
				std::cin >> num;
				switch (oper) {
				case '+':
					sum += num;
					break;
				case '-':
					sum -= num;
					break;
				case '*':
					sum *= num;
					break;
				case '/':
					sum /= num;
					break;
				default:
					std::cout << "ОШИБКА! Такого оператора нет!" << std::endl;
				}
			}
			std::cout << sum << std::endl;
		}

		return 0;
	}
	*/