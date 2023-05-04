
#include <string>

// ���������, ������� ������������ ���� ����: + - * / %  ( ) { }

const char NUMBER = '8';	// ��������� ������������ ���������� � ������������ ��� ����������� ����� � ����� ������.
const char EXHIBITOR = 'e';	// ��������� ������������ ���������� ��� ����������� ����� � ���������������� ����: 4.845e+03.
const char INITIAL = '=';	// ������������� ����������.
const char VARIABLES = 'v';	// �������� ����������� ����������.
const char FACTORIAL = '!';
const char EXIT = '#';

const std::string TITLE = "Calculator";
const std::string VERSION = "0.6.4";

const std::string INPUT = ">> "; // ����������� ������������ � �����.
const std::string RESULT = "= "; // ����� ������ ���������.


double expression();
double term();
double primary();
unsigned long int factorial(unsigned long int value);

void help();
void calculate();
