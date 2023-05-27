#include <iostream>
#include <string>
#include <locale.h>

#include "errors.h"
#include "constants.h"
#include "Token.h"
#include "Token_stream.h"
#include "Variables_stack.h"
#include "Information.h"
//#include "Functions.h"

/*
* Дошёл до реализации Римских чисел и запоролся. Решил переписать программу и в новой версии убрать глобальные переменные.
* 
 * Calculator
 * Калькулятор, который способен вычислять сложные выражения.
 * Автор Сергей Меденцов 20.05.2023-27.05.2023
 * version 0.8.1
 *
 * Что добавлено в этой версии:
 *  Добавлена возможность вывода интересной информации.
 *  Сделан рефакторинг кода
 *
 * Основные идеи проекта.
 *  Простой, но эффективный калькулятор, который может решать сложные примеры,
 *   например такие: 2+3.1*4=14.4, 10*(20+30)-100=400, 1+2*3/4%5+(6-7*(8))=-47.5
 *   или такие: variable=30, sky=24.5, res=variable+sky-4!, res // 30.5
 *   Так же можно сравнить два числа: 10 > 4, variable < sky, 10 == sky
 *
 * Как организован код.
 *
 * Какие сделаны предположения о вводных данных.
 * 
 * Какие фрагменты кода могут вызвать сложности.
 *
 * Каких фрагментов кода пока не хватает и какие варианты ещё не обработаны.
 *  Убрать по возможности глобальные переменные.
 *  Готовое решение формул! (Римские цифры | Меры длины | Меры массы | Количество диагоналей в многоугольнике)
 *  Ещё можно выводить циклы!
 *  Подумать, как можно выводить цифры без экспоненты и при переполнении типа значения тоже выводить корректные цифры.
*/
/*
* РАЗМЫШЛЕНИЯ НАД ЗАДАЧЕЙ:
* - Краткое изложение идеи, лежащей в основе решения.
* - Найти изъяны в своей первой идее.
* - Обсудить задачу и способы её решения с друзьями.
*/

/***********************************************************************************************************************************************/
// ПРОТОТИПЫ ФУНКЦИЙ

void     help();
void     launch();
bool     show_data(char ch);
void     show_tip();
void     show_result(int is_init, bool is_comp, double res);
void     comparison_result(double x);

double   initial(int& init, bool& comp);
double   comparison(bool& comp);
double   expression();
double   term();
double   primary();
long int factorial(long int value);
std::vector<double> get_function_params(double func_index);
double functions(std::string func_name, std::vector<double> params);

/***********************************************************************************************************************************************/

std::vector<char> specsymbols{ INITIAL, FACTORIAL, EXIT, NEXT, '+', '-', '*', '/', '%', '>', '<', '(', ')', '{', '}', '[', ']' };
std::vector<char> hotkeys{ SHOWVARS, SHOWCONSTS, SHOWFUNCS, HELP };

Token_stream ts;
Information info;
Variables_stack vs;

int main() {
	setlocale(LC_ALL, "Russian");

	help();

	launch();

	return 0;
}

/***********************************************************************************************************************************************/

// Функция выводит в консоль описание работы и горячие клавиши программы.
void help() {
	std::cout << "========================================================================================================================" << std::endl;
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
	std::cout << "Вы можете использовать любые английские буквы, для создания переменной. Пример: a " << INITIAL << " 10" << std::endl;
	std::cout << "А после использовать переменную в выражениях: a+40: " << RESULT << "50. 110 - a: " << RESULT << "100." << std::endl;
	std::cout << "Можно использовать не одну букву, а слова: Result " << INITIAL << " 20" << std::endl;
	std::cout << "Введите  [" << SHOWVARS << "] для просмотра всех определённых Вами переменных и их значений." << std::endl;
	std::cout << "------------------------------------------------------------------------------------------------------------------------" << std::endl;
	std::cout << "В программе есть несколько определённых константных переменных, которые нельзя изменить, таких как PI = 3.14" << std::endl;
	std::cout << "Введите  [" << SHOWCONSTS << "] для просмотра определённых констант." << std::endl;
	std::cout << "Так же есть несколько определённых функций, таких как вычислить корень выражения sqrt()" << std::endl;
	std::cout << "Введите  [" << SHOWFUNCS << "] для просмотра определённых функций." << std::endl;
	std::cout << "------------------------------------------------------------------------------------------------------------------------" << std::endl;
	std::cout << "Введите  [" << HELP << "] для вывода этой информации ещё раз." << std::endl;
	std::cout << "Введите  [" << EXIT << "] для выхода из программы." << std::endl;
	std::cout << "========================================================================================================================" << std::endl;
}

// Функция запрашивает ввод от пользователя, затем решает, как быть с этим вводом.
// Сначала проверяет, не является ли ввод пользователя специальной командой вывода данных или выхода из программы.
// Затем вызывает функцию парсинга введённой строки Token_stream::parsing(), которая отвечает за создание лексем.
// А затем вычисляет результат выражения с помощью грамматических функций, начиная с функции initial().
void launch() {
	while (true) {
		try {
			int init = 0;		// Если происходит инициализация переменной в программе, то счётчик увеличивается, потому как можно инициализировать несколько переменных.
			bool comp = false;	// Если происходит сравнение двух значений, то флаг станет равным true. За это отвечает функция comparison().
			std::string input;	// Ввод пользователя.

			std::cout << "\n" << INPUT;
			getline(std::cin, input);

			if (input.size() == 1)
			{
				bool res = show_data(input[0]);
				if (!res) break;
				// Если res будет равен одному, значит переданной команды не существует и следует продолжить выполнять функцию launch,
				// а не возвращаться к вводу пользователя.
				else if(res != 1) continue; 
			}
			if (input == "") {
				show_tip();
				continue;
			}
			if (!std::cin) throw ErrorInvalidInput{};

			ts.parsing(input);

//			ts.show_tokens();

			double res = initial(init, comp);
			show_result(init, comp, res);
		}
		// ОБЩИЕ ОШИБКИ
		catch (ErrorInvalidInput)        { std::cerr << "ОШИБКА! Неверный ввод выражения!\n"; }
		catch (ErrorUnknownToken)        { std::cerr << "ОШИБКА! Неизвестная лексема!\n"; }
		catch (ErrorMissingLeftBracket)  { std::cerr << "ОШИБКА! Нет открывающей скобки!\n"; }
		catch (ErrorMissingRightBracket) { std::cerr << "ОШИБКА! Нет закрывающей скобки!\n"; }
		catch (ErrorNoLeftExpression)    { std::cerr << "ОШИБКА! Нет начального значения!\n"; }
		catch (ErrorUseNext)             { std::cerr << "ОШИБКА! Чтобы присвоить значения нескольким переменным в одной строке используйте символ [" << NEXT << "] между выражениями!\n"; }
		catch (ErrorNoMultiExpression)   { std::cerr << "ОШИБКА! В одной строке можно выполнить одно выражение, если выражение не является присвоением значения переменной!\n"; }
		// ОШИБКИ ПЕРЕМЕННЫХ, КОНСТАНТ И ФУНКЦИЙ
		catch (ErrorNoVariable)          { std::cerr << "ОШИБКА! Переменная не определена!\n"; }
		catch (ErrorDefinedVariables)    { std::cerr << "ОШИБКА! Невозможно отобразить определённые в программе переменные!\n"; }
		catch (ErrorConstantInitial)     { std::cerr << "ОШИБКА! Нельзя присвоить значение константе. Данное имя используется системой.\n"; }
		catch (ErrorFunctionInitial)     { std::cerr << "ОШИБКА! Нельзя присвоить значение функции. Данное имя используется системой.\n"; }
		catch (ErrorIvalidEqually)       { std::cerr << "ОШИБКА! Не верное присваивание!\n"; }
		// ОШИБКИ ЧИСЕЛ
		catch (ErrorDivisionZero)        { std::cerr << "ОШИБКА! Деление на ноль!\n"; }
		catch (ErrorDigitalNotEqually)   { std::cerr << "ОШИБКА! Число не может хранить значение. Присвойте значение переменной!\n"; }
		catch (ErrorNegative)            { std::cerr << "ОШИБКА! Передано отрицательное значение!\n"; }
		catch (ErrorOverflowValue)       { std::cerr << "ОШИБКА! Переполнение значения факториала!\n"; }
		catch (ErrorBadRomanInput)       { std::cerr << "ОШИБКА! Неверный ввод Римских цифр!\n"; }
		catch (ErrorRomanNotCorrect)     { std::cerr << "ОШИБКА! Функция roman() ожидает либо Римские либо Арабские цифры!\n"; }
		catch (ErrorNotArgumentFunction) { std::cerr << "ОШИБКА! Функция ожидает аргумента. Пропущен аргумент функции.\n"; }
		// ОШИБКИ СРАВНЕНИЯ
		catch (ErrorNotComparison)       { std::cerr << "ОШИБКА! Сравнить можно только два числа или две переменных! \nРезультат сравнения не может быть присвоен переменной!\n"; }
		// ОШИБКИ РАБОТЫ С ФАЙЛАМИ
		catch (ErrorNotFile)             { std::cerr << "ОШИБКА! Не возможно прочитать файл с советами: files/tips.txt\n"; }
		catch (ErrorEmptyFile)           { std::cerr << "ОШИБКА! Файл с советами пустой!\n"; }
	}
}

// char ch - символ, который означает горячую клавишу.
// Специальные команды вывода данных, таких как определённых переменных, констант или функций.
// Функция вернёт 1, если переданной команды не существует и 0, если команда означает выход из программы.
// Иначе show_data() вызовет специальную функцию, соответствующую переданному символу.
bool show_data(char ch) {
	if (ch == EXIT) return false;
	switch (ch) {
	case SHOWVARS:
		vs.show_variables(SHOWVARS);
		break;
	case SHOWCONSTS:
		vs.show_variables(SHOWCONSTS);
		break;
	case SHOWFUNCS:
		vs.show_variables(SHOWFUNCS);
		break;
	case HELP:
		help();
		break;
	default:
		return true;
	}
}

// Выводит рандомные советы, описанные в классе Information, в консоль.
void show_tip() {
	std::cout << "ИНТЕРЕСНАЯ ИНФОРМАЦИЯ:" << std::endl;
	std::cout << "------------------------------------------------------------------------------------------------------------------------" << std::endl;
	std::cout << info.get_tip();
	std::cout << "------------------------------------------------------------------------------------------------------------------------" << std::endl;
}

// int is_init - целочисленное значение, содержащее количество инициализированных переменных. ini(Initialization)
// bool is_comp - было ли произведено сравнение чисел? comp(Comparison)
// res - аргумент содержащий результат выражения и предназначен для вывода в консоль. res(Result)
// Выводит в консоль результат выражения или надпись о выполнении программой определённого действия, например "инициализации переменной".
void show_result(int is_init, bool is_comp, double res) {
	if (is_comp) {
		comparison_result(res);
		return;
	}
	if (!is_init)
		std::cout << RESULT << res << std::endl;
	else {
		if (is_init == 1)
			std::cout << "Переменная инициализирована. ";
		else 
			std::cout << "Переменные инициализированы. ";
		std::cout << "Чтобы посмотреть список имеющихся переменных введите символ[" << SHOWVARS << "]\n";
	}
}

// x - это значение типа double, но представляет из себя булево выражение 1-true, 0-false.
// Выводит в стандартный поток вывода ответ, верно ли сравнение.
void comparison_result(double x) {
	if (x) std::cout << RESULT << "Сравнение верно"    << std::endl;
	else   std::cout << RESULT << "Сравнение не верно" << std::endl;
}

// ch - любой символ.
// Определяет, есть ли переданный параметру символ в глобальном массиве спецсимволов.
// Возвращает true - если есть, false - если нет.
bool isspecsymbol(char ch) {
	for (char c : specsymbols) {
		if (c == ch) return true;
	}
	return false;
}

// ch - любой символ.
// Определяет, есть ли переданный параметру символ в глобальном массиве горячих клавиш.
// Возвращает true - если есть, false - если нет.
bool ishotkey(char ch) {
	for (char c : hotkeys) {
		if (c == ch) return true;
	}
	return false;
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
*   Константа
*	Число
*   -Число
*   +Число
*	"(" Выражение ")"				// Группировка
*   FUNCTION(Первичное_выражение)	// Определённая функция
* Константа:
*   строковый_литерал
* Переменная:
*   строковый_литерал
* Число:
*	Литерал_с_плавающей_точкой
*/

// &init - счётчик инициализации переменной. То есть, если инициализируется переменная, то переменная увеличивается на один.
// &comp - флаг, принимающий значение true, если выражение содержало сравнение чисел.
// Определяет, было ли присвоено выражение переменной или нет.
// Меняет значение своих параметров.
double initial(int& init, bool &comp) {
	Token t = ts.get();
	if (t.kind == INITVAR) {
		std::string name_var = ts.get_buffer_name(t.value);
		t = ts.get();
		double d = comparison(comp);
		vs.add_variable(name_var, d);
		init++;
		ts.putback();
		t = ts.get();
		if (t.kind == NEXT) {
			initial(init, comp);
		}
	}
	else if (t.kind == VARIABLE) {
		int index = t.value;
		t = ts.get();
		if (t.kind == INITIAL) {
			double d = comparison(comp);
			vs.add_variable(vs.get_name(index), d);
			init++;
			ts.putback();
			t = ts.get();
			if (t.kind == NEXT) {
				initial(init, comp);
			}
		}
		else {
			if (t.kind != EXIT) ts.putback();
			ts.putback();
			return comparison(comp);
		}
	}
	else if (t.kind == EXIT) {
		return 0;
	}
	else {
		if (t.kind != EXIT) ts.putback();
		return comparison(comp);
	}
}

// comp - булев тип, отвечающий на вопрос, является ли лексема сравнением чисел.
// Проверяет, является ли лексема сравнением чисел и работает с операторами '>', '<', РАВНО, ЗАПЯТОЙ.
// Если два выражения, отделённые запятой, не являются инициализацией переменной, то выбрасывает исключение.
// Возвращает результат сравнения 0 - false | 1 - true | число, если операция не является сравнением.
double comparison(bool &comp) {
	double left = expression();
	Token t;
	while (true) {
		t = ts.get();
		switch (t.kind) {
		case '>':
			comp = true;
			return left > expression();
		case '<':
			comp = true;
			return left < expression();
		case EQUALLY:
			comp = true;
			return left == expression();
		case NEXT:
			t = ts.get();
			if (t.kind == VARIABLE || t.kind == INITVAR) {
				ts.putback();
				return left;
			}
			throw ErrorNoMultiExpression{};
		default:
			return left;
		}
	}
}

// Функция проверяет, является ли лексема выражением и возвращает значение.
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

// Функция проверяет, является ли лексема слагаемым и возвращает значение слагаемого.
// И работает с операторами '*', '/', '%' и факториал.
// Если лексема не подходит не под одну константу, то возвращает значени.
double term() {
	double left = primary();
	Token t;
	while (true) {
		t = ts.get();
		switch (t.kind) {
		case '*':
			left *= primary();
			break;
		case '/': {
			double digit = primary();
			if (digit == 0) {
				ts.get();
				throw ErrorDivisionZero{};
			}
			left /= digit;
			break;
		}
		case '%': {
			double digit = primary();
			if (digit == 0) {
				throw ErrorDivisionZero{};
			}
			double res = (int)left / (int)digit;
			left = left - (res * digit);
			break;
		}
		case FACTORIAL:
			return factorial((unsigned long int)left);
		default:
			ts.putback();
			return left;
		}
	}
}

// Функция проверяет, является ли лексема первичным выражением и возвращает это значение.
// Если лексема является вызовом функции, то вызывает эту функцию.
// Если лексема является скобками, то вычисляет выражение в скобках и возвращает значение.
// Если нет закрывающей скобки, выбрасывает исключение.
// Если лексема не является частью программы, то выбрасывает исключение.
double primary() {
	Token t = ts.get();
	switch (t.kind) {
	case NUMBER:
		return t.value;
	case CONSTANTS:
	case VARIABLE:
		return vs.get_variable(t.value);
	case ROMAN:
		return 0;
	case '-':
		return -primary();
	case '+':
		return primary();
	case '(':
	case '{': {
		double res = expression();
		t = ts.get();
		if (t.kind != ')' && t.kind != '}') throw ErrorMissingRightBracket{};
		return res;
	}
	case FUNCTIONS: {
		std::string func_name = vs.get_name(t.value);
		std::vector<double> params = get_function_params(t.value);
		t = ts.get();
		if (t.kind != ')') throw ErrorMissingRightBracket{};
		return functions(func_name, params);
	}
	default:
		throw ErrorInvalidInput{};
	}
}

// func_index - индекс, по которому константная функция находится в массиве констант.
// Возвращает значения параметров, передаваемые в константную функцию.
std::vector<double> get_function_params(double func_index) {
	std::vector<double> params;
	int func_args = vs.get_variable(func_index);
	int args_index = 0;
	while (args_index < func_args) {
		Token t = ts.get();
		if (t.kind == NEXT) continue;
		if (args_index > 0) ts.putback();
		params.push_back(expression());
		args_index++;
	}
	return params;
}

// func_name - название функции, которую следует вызвать из стека переменных.
// params - значения аргументов вызываемой функции.
// По названию функции подставляет переданные параметры и возвращает результат работы функции.
double functions(std::string func_name, std::vector<double> params) {
	//Functions f;
	if (func_name == "sqrt") {
		if (params[0] < 1) throw ErrorNegative{};
		return sqrt(params[0]);
	}
	else if (func_name == "pow") {
		return pow(params[0], params[1]);
	}
	/*else if (func_name == "roman") {
		return f.roman(vs.get_roman());
	}*/
}

// value - число, большее или равное 0.
// Возвращает факториал числа.
// Если число меньше ноля или превышает максимально допустимое число системы, то выбросит исключение.
long int factorial(long int value) {
	if (value == 0) return 1;
	if (value < 0) throw ErrorNegative{};
	for (int count = value; count > 1;) {
		value *= --count;
	}
	if (value <= 0) throw ErrorOverflowValue{};
	return value;
}
/***********************************************************************************************************************************************/