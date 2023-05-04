
#include <string>

// Константы, которые представляют сами себя: + - * / %  ( ) { }

const char NUMBER = '8';	// Константа используется программой и используется для обозначения цифры в стеке лексем.
const char EXHIBITOR = 'e';	// Константа используется программой для определения числа в экспоненциальном виде: 4.845e+03.
const char INITIAL = '=';	// Инициализация переменной.
const char VARIABLES = 'v';	// Просмотр определённых переменных.
const char FACTORIAL = '!';
const char EXIT = '#';

const std::string TITLE = "Calculator";
const std::string VERSION = "0.6.4";

const std::string INPUT = ">> "; // Приглашение пользователя к вводу.
const std::string RESULT = "= "; // Вывод ответа выражения.


double expression();
double term();
double primary();
unsigned long int factorial(unsigned long int value);

void help();
void calculate();
