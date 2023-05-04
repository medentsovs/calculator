#pragma once

// Представляет тип лексемы для парсинга введённой строки.
class Token {
public:
	// Вид лексемы.
	char kind;
	// Значение лексемы.
	double value;
};
