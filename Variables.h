#pragma once

#include <iostream>

// Объект переменных.
class Variables
{
public:
	// Название переменной
	std::string name = "";
	// Значение переменной
	double value;
	// Тип переменной: [V]ariable | [C]onstat | [F]unction
	char type;
	// Описание переменной. Это больше нужно для функций.
	std::string comment;
};