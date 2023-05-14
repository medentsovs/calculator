﻿#include <iostream>
#include <string>
#include <locale.h>

#include "errors.h"
#include "constans.h"
#include "Token_stream.h"
#include "Variables_stack.h"

/*
 * Calculator
 * Калькулятор, который способен вычислять сложные выражения.
 * Автор Сергей Меденцов 12.05.2023-14.05.2023
 * version 0.7.3
 *
 * Что добавлено в этой версии:
 *  Добавлена возможность сравнивать числа и переменные с помощью операторов > < ==
 *
 * Основные идеи проекта.
 *  Простой, но эффективный калькулятор, который может решать сложные примеры,
 *   например такие: 2+3.1*4=14.4, 10*(20+30)-100=400, 1+2*3/4%5+(6-7*(8))=-47.5
 *   или такие: variable=30, sky=24.5, res=variable+sky-4!, res // 30.5
 *   Так же можно сравнить два числа: 10 > 4, variable < sky, 10 == sky
 *
 * Как организован код.
 *  Программа выводит информацию о программе и ожидает ввода выражения,
 *   затем парсит введённую строку с помощью функции Token_stream::parsing(), разбивая её на лексемы.
 *   Лексемы делятся на ЧИСЛА, ПЕРЕМЕННЫЕ и ОПЕРАТОРЫ.
 *   Лексемы представлены классом Token и содержат вид лексемы и её значение.
 *  Считанные лексемы вычисляются с помощью функций, реализующих грамматику выражений, описанную ниже.
 *  Ошибки описаны в заголовочном файле errors.
 *   При выбрасывании исключения программа выводит сообщение в консоль и продолжает работу.
 *  Константы прописаны в файле constans.h
 *
 * Какие сделаны предположения о вводных данных.
 *  Вводные данные должны быть одной строкой, содержащей цифры, буквы и знаки операторов: + - / * % ( ) . , { } > < == = !
 *   Программа парсит введённую строку и разбивает её на части.
 * 
 * Какие фрагменты кода могут вызвать сложности.
 *  Функция Token_stream::parsing()
 *  На эту функцию возложена задача разложить вводимую пользователем строку на лексемы, 
 *   которые в дальнейшем используются для вычисления выражения.
 *  Так как возможностей выбора у пользователя много, приходится разбивать введённую строку на множество лексем,
 *   поэтому функция получилась огромной. Я разбил её на мелкие функции и закомментировал, как мог)),
 *    но всё же она может запутать и чтобы понять её логику придётся немного посидеть над ней.
 *     Возможно в будущем постараюсь её упростить, но пока не представляю, как это сделать.
 *
 * Каких фрагментов кода пока не хватает и какие варианты ещё не обработаны.
 *  Если просто нажать Enter можно выводить какую-нибудь интересную информацию)
 *  Определить константы.
 *  Готовое решение формул!
 *  Ещё можно выводить циклы!
 *  Подумать, как можно выводить цифры без экспоненты и при переполнении типа значения тоже выводить корректные цифры.
*/
/*
* РАЗМЫШЛЕНИЯ НАД ЗАДАЧЕЙ:
*  ОПРЕДЕЛИТЬ КОНСТАНТЫ.
* - Краткое изложение идеи, лежащей в основе решения.
* - Найти изъяны в своей первой идее.
* - Обсудить задачу и способы её решения с друзьями.
*/

/***********************************************************************************************************************************************/
// ПРОТОТИПЫ ФУНКЦИЙ

void   help();
void   calculate();
void   comparison_result(double x);
//-------------------------------------------------------
double initial(bool& init, bool& comp);
double comparison(bool& comp);
double expression();
double term();
double primary();
unsigned long int factorial(unsigned long int value);

/***********************************************************************************************************************************************/

Token_stream ts;
Variables_stack vs;

int main() {
	setlocale(LC_ALL, "Russian");

	help();

	calculate();

	return 0;
}

/***********************************************************************************************************************************************/
// Описание работы программы.
void help() {
	std::cout << TITLE << ". version " << VERSION << std::endl;
	std::cout << "________________________________________________________________________________________________________________________" << std::endl;
	std::cout << "Вы можете ввести любое выражение, используя операторы [+][-][*][/]. Для получения ответа введите [Enter]." << std::endl;
	std::cout << "Так же можно использовать скобки [(][)][{][}] для указания порядка вычисления. Скобки идентичны, но не взаимозаменяемы!" << std::endl;
	std::cout << "Оператор [%] используется для вычисления остатка от деления." << std::endl;
	std::cout << "Оператор [" << FACTORIAL << "] используется для вычисления факториала и указывается после числа, факториал которого необходимо вычислить." << std::endl;
	std::cout << "------------------------------------------------------------------------------------------------------------------------" << std::endl;
	std::cout << "Сравнить два числа или две переменные можно с помощью операторов [>][<][" << INITIAL << INITIAL << "]." << std::endl;
	std::cout << "Присвоить значение сравнения переменной нельзя!" << std::endl;
	std::cout << "------------------------------------------------------------------------------------------------------------------------" << std::endl;
	std::cout << "Вы можете использовать любые строчные английские буквы, для создания переменной. Пример: a " << INITIAL << " 10" << std::endl;
	std::cout << "А после использовать переменную в выражениях: a+40: " << RESULT << "50. 110 - a: " << RESULT << "100." << std::endl;
	std::cout << "Можно использовать не одну букву, а слова: result " << INITIAL << " 20" << std::endl;
	std::cout << "Нельзя использовать цифры в объявлении переменных: var1 " << INITIAL << " 30" << std::endl;
	std::cout << "Введите  [" << SHOWVARS << "] для просмотра всех определённых переменных и их значений." << std::endl;
	std::cout << "------------------------------------------------------------------------------------------------------------------------" << std::endl;
	std::cout << "Введите  [" << HELP << "] для вывода этой информации." << std::endl;
	std::cout << "Введите  [" << EXIT << "] для выхода из программы." << std::endl;
	std::cout << "________________________________________________________________________________________________________________________" << std::endl;
}
// ПОЛУЧЕНИЕ ВВОДНЫХ ДАННЫХ и Вызов функций обработчиков.
void calculate() {
	while (true) {
		try {
			bool init = false;	// Флаг инициализации переменной.
			bool comp = false;	// Флаг сравнения чисел.
			std::string input;	// Ввод пользователя.

			std::cout << "\n" << INPUT;
			getline(std::cin, input);

			if (*input.c_str() == EXIT) break;
			if (*input.c_str() == SHOWVARS) {
				vs.show_variables();
				continue;
			}
			if (*input.c_str() == HELP) {
				help();
				continue;
			}
			if (input == "") {
				std::cout << "Здесь будет интересная информация!\n";
				continue;
			}
			if (!std::cin) throw ErrorInvalidInput{};
			ts.parsing(input, vs);

			double x = initial(init, comp);
			if (comp) {
				comparison_result(x);
				comp = false;
				continue;
			}
			if(!init)
				std::cout << RESULT << x << std::endl;
			else {
				std::cout << "Переменная инициализирована\n";
			}
		}
		catch (ErrorUnknownToken)        { std::cerr << "ОШИБКА! Неизвестная лексема!\n"; }
		catch (ErrorInvalidInput)        { std::cerr << "ОШИБКА! Неверный ввод выражения!\n"; }
		catch (ErrorMissingRightBracket) { std::cerr << "ОШИБКА! Нет закрывающей скобки!\n"; }
		catch (ErrorMissingLeftBracket)  { std::cerr << "ОШИБКА! Нет открывающей скобки!\n"; }
		catch (ErrorNegativeFactorial)   { std::cerr << "ОШИБКА! Отрицательный факториал!\n"; }
		catch (ErrorOverflowValue)       { std::cerr << "ОШИБКА! Переполнение значения факториала!\n"; }
		catch (ErrorNoLeftExpression)    { std::cerr << "ОШИБКА! Нет начального значения!\n"; }
		catch (ErrorDivisionZero)        { std::cerr << "ОШИБКА! Деление на ноль!\n"; }
		catch (ErrorNoVariable)          { std::cerr << "ОШИБКА! Переменная не определена!\n"; }
		catch (ErrorNumberVariable)      { std::cerr << "ОШИБКА! Название переменной не может содержать цифры!\n"; }
		catch (ErrorNotComparison)       { std::cerr << "ОШИБКА! Сравнить можно только два числа или две переменных! \nРезультат сравнения не может быть присвоен переменной!\n"; }
	}
}

// Выводит в стандартный поток вывода ответ, верно ли сравнение.
// Аргумент - это булево выражение 1-true, 0-false.
void comparison_result(double x) {
	if (x) {
		std::cout << RESULT << "Сравнение верно" << std::endl;
	}
	else {
		std::cout << RESULT << "Сравнение не верно" << std::endl;
	}
}
/***********************************************************************************************************************************************/

/***********************************************************************************************************************************************/
// ФУНКЦИИ, РЕАЛИЗУЮЩИЕ ГРАММАТИКУ ВЫРАЖЕНИЙ

/*	ГРАММАТИКА ВЫРАЖЕНИЙ:
* --------------------------------------------------------------------
* Инициализация:
*   Переменная "=" Сравнение
* Сравнение:
*   Выражение
*   Выражение > Выражение
*   Выражение < Выражение
*   Выражение = Выражение
* Выражение:
*	Терм
*	Выражение "+" Терм	// Сложение
*	Выражение "-" Терм	// Вычитание
* Терм:
*	Первичное_выражение
*	Первичное_выражение!			// Факториал
*	Терм "*" Первичное_выражение	// Умножение
*	Терм "/" Первичное_выражение	// Деление
*	Терм "%" Первичное_выражение	// Остаток (деление по модулю)
* Первичное_выражение:
*   Переменная
*	Число
*   -Число
*   +Число
*	"(" Выражение ")"	// Группировка
* Переменная:
*   строковый_литерал
* Число:
*	Литерал_с_плавающей_точкой
*/

// Инициализация переменной.
double initial(bool& init, bool &comp) {
	Token t = ts.get();
	if (t.kind == EXIT) {
		return vs.get_last_number();
	}
	if (t.kind == VARIABLE) {
		int index = t.value;
		t = ts.get();
		if (t.kind == INITIAL) {
			double d = comparison(comp);
			vs.add_variable(vs.get_name(index), d);
			init = true;
			ts.index_reset();
			return d;
		}
		else {
			return vs.get_variable(index);
		}
	}
	else {
		ts.putback();
		double d = comparison(comp);
		ts.index_reset();
		return d;
	}
}
// Сравнение выражений.
double comparison(bool &comp) {
	double d = expression();
	Token t;
	while (true) {
		t = ts.get();
		switch (t.kind) {
		case '>':
			comp = true;
			return d > expression();
		case '<':
			comp = true;
			return d < expression();
		case EQUALLY:
			comp = true;
			return d == expression();
		default:
			return d;
		}
	}
}
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
			ts.putback();
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
				ts.get();
				throw ErrorDivisionZero{};
			}
			left /= p;
			break;
		}
		case '%':
		{
			double p = primary();
			if (p == 0) {
				ts.get();
				throw ErrorDivisionZero{};
			}
			double res = (int)left / (int)p;
			left = left - (res * p);
			break;
		}
		case FACTORIAL:
			return factorial(left);
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
	case NUMBER:
	{
		return t.value;
	}
	case VARIABLE:
	{
		int index = t.value;
		return vs.get_variable(index);
	}
	case '-':
		return -primary();
	case '+':
		return primary();
	case '(':
	case '{':
	{
		double d = expression();
		ts.putback();
		t = ts.get();
		if (t.kind != ')' && t.kind != '}') throw ErrorMissingRightBracket{};
		return d;
	}
	default:
		throw ErrorInvalidInput{};
	}
}
// Факториал
unsigned long int factorial(unsigned long int value) {
	if (value == 0) return 1;
	if (value < 0) throw ErrorNegativeFactorial{};
	for (int count = value; count > 1;) {
		value *= --count;
	}
	if (value <= 0) throw ErrorOverflowValue{};
	return value;
}
/***********************************************************************************************************************************************/