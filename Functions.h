#pragma once
#include <string>
#include <vector>
#include "errors.h"
#include "Token.h"
#include "Variables_stack.h"

extern Variables_stack vs;

static class Functions
{
	std::vector<Token> roman_numeric{
		{ 'I',    1 },
		{ 'V',    5 },
		{ 'X',   10 },
		{ 'L',   50 },
		{ 'C',  100 },
		{ 'D',  500 },
		{ 'M', 1000 }
	};

	/// <summary>
	/// Проверяет, является ли переданная строка Римским числом.
	/// </summary>
	/// <param name="roman"> - число в Римском исчислении</param>
	/// <returns>true, если аргумент является Римским числом, иначе - false</returns>
	bool check_roman(std::string roman) {
		for (char ch : roman) {
			if (get_value(ch) == -1)
				return false;
		}
		return true;
	}
	/// <summary>
	/// Находит арабское значение переданного римского значения аргумента.
	/// </summary>
	/// <param name="roman_symbol"> - символ Римской цифры</param>
	/// <returns>Значение Римской цифры в арабской системе исчисления. Возвращает -1, если переданный символ не найден в массиве символов.</returns>
	int get_value(char roman_symbol) {
		for (Token t : roman_numeric) {
			if (t.kind == roman_symbol)
				return t.value;
		}
		return -1;
	}
	/// <summary>
	/// Проверяет массив с римскими символами и находит число равное или меньшее переданного аргумента.
	/// </summary>
	/// <param name="arabian_number"> - число в арабской системе исчисления</param>
	/// <returns>Римский символ, по значению равный или меньший значения аргумента.</returns>
	char get_roman_symbol(int arabian_number) {
		for (Token t : roman_numeric) {
			if (arabian_number >= t.value)
				return t.kind;
		}
	}










	// num - арабское число.
	// Функция возвращает максимальное число в массиве roman_numeric не превышающее переданный параметр.
	int get_max(int num) {
		for (int i = 0; i < roman_numeric.size(); i++) {
			if (num == roman_numeric[i].value)
				return roman_numeric[i].value;
			else if (num > roman_numeric[i].value)
				return roman_numeric[i - 1].value;
		}
	}
	// num - арабское число.
	// Возвращает символ римской цифры большей на порядок переданного параметра.
	char get_max_next(char num) {
		for (int i = 0; i < roman_numeric.size(); i++) {
			if (num == roman_numeric[i].value)
				return roman_numeric[i + 1].kind;
		}
	}
	// number - константная строка, представляющая собой римское число.
	// Возвращает true, если переданная строка представляет собой римское число.
	// Если в переданной строке содержатся символы не являющиеся римским символом цифры выбрасывает исключение.
	/*bool is_roman(const std::string& number) {
		for (int i = 0; i < number.size(); i++) {
			if (!check_roman(number[i])) {
				vs.reset_roman();
				throw ErrorBadRomanInput{};
			}
		}
		return true;
	}*/
	// number - константная строка, представляющая собой арабское число.
	// Возвращает арабское число, преобразованное из строки, если все символы представляют собой арабские цифры.
	// Выбрасывает исключение, если кокой-то символ не является арабской цифрой.
	int is_arabian(const std::string& number) {
		for (int i = 0; i < number.size(); i++) {
			if (!isdigit(number[i])) {
				vs.reset_roman();
				throw ErrorRomanNotCorrect{};
			}
		}
		return atoi(number.c_str());
	}
	// number - константная строка, представляющая собой римское число.
	// Функция проверяет, является ли переданная строка римским числом
	// и производит преобразование римского числа в арабское.
	// Очищает переменную roman в объекте Variables_stack и возвращает арабское число.
	/*double get_arabian(const std::string& number) {
		double count = 0;
		is_roman(number);
		count = get_value(number[number.size() - 1]);
		for (int i = number.size() - 1; i > 0; i--) {
			if (get_value(number[i]) > get_value(number[i - 1]))
				count -= get_value(number[i - 1]);
			else
				count += get_value(number[i - 1]);
		}
		vs.reset_roman();
		return count;
	}*/
	/*
		Найти максимальное значение на которое можно разделить число. MAX = REMAINDER
		Разделить число на максимальное значение. Получим количество повторов. REPEAT = NUM / MAX
		Если количество повторов больше трёх, значит одна цифра должна вычитаться.
		 Если ROMAN не пустой
		  Из ROMAN удаляем последнюю цифру, узнав её значение. CURRENT
		 Если ROMAN пустой
		  CURRENT = MAX
		 Берём максимальную цифру большую CURRENT. MAX = CURRENT->next
		 Добавляем в ROMAN MAX
		Иначе
		 Запишем в ROMAN римскую цифру, соответствующую MAX, REPEAT раз.
		Делим по модулю число на максимальное значение. Получим остаток от числа. REMAINDER = NUM % MAX
		Проверяем, если REMAINDER = 0,
		 значит всё готово. Число разобрано.
		Если не равен 0,
		 значит повторяем всё с самого начала.
		*/
	//double get_roman(const std::string& number) {
	//	// В Римских числах один символ можно повторить 3 раза III, не более.
	//	const int max_repeat = 3; // Количество повторов разрешённое в римских числах.
	//	int num = is_arabian(number);
	//	while (true) {
	//		int remainder = get_max(num);
	//		int repeat = num / remainder;
	//		char current = '_';
	//		if (repeat > max_repeat) {
	//			if (vs.get_roman() == "") {
	//				current = remainder;
	//			}
	//			else {
	//				current = vs.roman_decrement();
	//			}
	//			vs.add_roman(get_max_next(current));
	//		}
	//		else {

	//		}
	//	}
	//	


	//	return 0;
	//}
public:
	/// <summary>
	/// Функция проверяет, в какой системе счисления ей передано значение (в Римской или Арабской) 
	/// и вызывает функцию для перевода полученного числа в противоположную систему исчисления.
	/// Несмотря на то, что при передаче арабского значения
	/// </summary>
	/// <param name="number"> - число в Римской или Арабской системе исчисления.</param>
	/// <returns>Число в арабском исчислении.</returns>
	/*double roman(std::string number) {
		if (isdigit(number[0])) {
			return get_roman(number);
		}
		else if (get_value(number[0])) {
			return get_arabian(number);
		}
		else {
			throw ErrorInvalidInput{};
		}
	}*/
};

