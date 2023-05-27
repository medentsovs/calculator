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
	/// ���������, �������� �� ���������� ������ ������� ������.
	/// </summary>
	/// <param name="roman"> - ����� � ������� ����������</param>
	/// <returns>true, ���� �������� �������� ������� ������, ����� - false</returns>
	bool check_roman(std::string roman) {
		for (char ch : roman) {
			if (get_value(ch) == -1)
				return false;
		}
		return true;
	}
	/// <summary>
	/// ������� �������� �������� ����������� �������� �������� ���������.
	/// </summary>
	/// <param name="roman_symbol"> - ������ ������� �����</param>
	/// <returns>�������� ������� ����� � �������� ������� ����������. ���������� -1, ���� ���������� ������ �� ������ � ������� ��������.</returns>
	int get_value(char roman_symbol) {
		for (Token t : roman_numeric) {
			if (t.kind == roman_symbol)
				return t.value;
		}
		return -1;
	}
	/// <summary>
	/// ��������� ������ � �������� ��������� � ������� ����� ������ ��� ������� ����������� ���������.
	/// </summary>
	/// <param name="arabian_number"> - ����� � �������� ������� ����������</param>
	/// <returns>������� ������, �� �������� ������ ��� ������� �������� ���������.</returns>
	char get_roman_symbol(int arabian_number) {
		for (Token t : roman_numeric) {
			if (arabian_number >= t.value)
				return t.kind;
		}
	}










	// num - �������� �����.
	// ������� ���������� ������������ ����� � ������� roman_numeric �� ����������� ���������� ��������.
	int get_max(int num) {
		for (int i = 0; i < roman_numeric.size(); i++) {
			if (num == roman_numeric[i].value)
				return roman_numeric[i].value;
			else if (num > roman_numeric[i].value)
				return roman_numeric[i - 1].value;
		}
	}
	// num - �������� �����.
	// ���������� ������ ������� ����� ������� �� ������� ����������� ���������.
	char get_max_next(char num) {
		for (int i = 0; i < roman_numeric.size(); i++) {
			if (num == roman_numeric[i].value)
				return roman_numeric[i + 1].kind;
		}
	}
	// number - ����������� ������, �������������� ����� ������� �����.
	// ���������� true, ���� ���������� ������ ������������ ����� ������� �����.
	// ���� � ���������� ������ ���������� ������� �� ���������� ������� �������� ����� ����������� ����������.
	/*bool is_roman(const std::string& number) {
		for (int i = 0; i < number.size(); i++) {
			if (!check_roman(number[i])) {
				vs.reset_roman();
				throw ErrorBadRomanInput{};
			}
		}
		return true;
	}*/
	// number - ����������� ������, �������������� ����� �������� �����.
	// ���������� �������� �����, ��������������� �� ������, ���� ��� ������� ������������ ����� �������� �����.
	// ����������� ����������, ���� �����-�� ������ �� �������� �������� ������.
	int is_arabian(const std::string& number) {
		for (int i = 0; i < number.size(); i++) {
			if (!isdigit(number[i])) {
				vs.reset_roman();
				throw ErrorRomanNotCorrect{};
			}
		}
		return atoi(number.c_str());
	}
	// number - ����������� ������, �������������� ����� ������� �����.
	// ������� ���������, �������� �� ���������� ������ ������� ������
	// � ���������� �������������� �������� ����� � ��������.
	// ������� ���������� roman � ������� Variables_stack � ���������� �������� �����.
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
		����� ������������ �������� �� ������� ����� ��������� �����. MAX = REMAINDER
		��������� ����� �� ������������ ��������. ������� ���������� ��������. REPEAT = NUM / MAX
		���� ���������� �������� ������ ���, ������ ���� ����� ������ ����������.
		 ���� ROMAN �� ������
		  �� ROMAN ������� ��������� �����, ����� � ��������. CURRENT
		 ���� ROMAN ������
		  CURRENT = MAX
		 ���� ������������ ����� ������� CURRENT. MAX = CURRENT->next
		 ��������� � ROMAN MAX
		�����
		 ������� � ROMAN ������� �����, ��������������� MAX, REPEAT ���.
		����� �� ������ ����� �� ������������ ��������. ������� ������� �� �����. REMAINDER = NUM % MAX
		���������, ���� REMAINDER = 0,
		 ������ �� ������. ����� ���������.
		���� �� ����� 0,
		 ������ ��������� �� � ������ ������.
		*/
	//double get_roman(const std::string& number) {
	//	// � ������� ������ ���� ������ ����� ��������� 3 ���� III, �� �����.
	//	const int max_repeat = 3; // ���������� �������� ����������� � ������� ������.
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
	/// ������� ���������, � ����� ������� ��������� �� �������� �������� (� ������� ��� ��������) 
	/// � �������� ������� ��� �������� ����������� ����� � ��������������� ������� ����������.
	/// �������� �� ��, ��� ��� �������� ��������� ��������
	/// </summary>
	/// <param name="number"> - ����� � ������� ��� �������� ������� ����������.</param>
	/// <returns>����� � �������� ����������.</returns>
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

