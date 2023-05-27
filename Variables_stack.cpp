#include "Variables.h"
#include "Variables_stack.h"

// Определяет, есть ли в базе определённые пользователем переменные.
// Возвращает true, если переменные были определены, иначе возвращает false.
const bool Variables_stack::is_defined_variables() {
	for (Variables v : variables)
		if (v.type == 'V') return true;
	return false;
}

// type - тип лексемы: переменная, константа или функция.
// В зависимости от переданного символа возвращает заголовок выводимых переменных.
// Если передана неизвестная лексема, то выбрасывает исключение.
const std::string Variables_stack::show_title_variables(char type) {
	switch (type) {
	case 'V':
		if (is_defined_variables()) return "Определённые переменные:";
		else                        return "Переменные не определены!";
	case 'C':                       return "Определённые константы:";
	case 'F':                       return "Определённые функции:";
	default:                        throw ErrorDefinedVariables{};
	}
}

// type - тип лексемы: переменная, константа или функция.
// index - индекс переменной в стеке переменных.
// В зависимости от переданного символа возвращает строку значения переменной или комментарий функции.
const std::string Variables_stack::show_footer_variables(char type, int index) {
	switch (type) {
	case 'V': case 'C': return " = " + std::to_string(variables[index].value);
	case 'F':           return variables[index].comment;
	default:            throw ErrorDefinedVariables{};
	}
}

// type - тип лексемы: переменная, константа или функция.
// В зависимости от переданного символа выводит в стандартный поток вывода определённые в системе переменные.
const void Variables_stack::show_variables(char type) {
	std::cout << show_title_variables(type) << std::endl;
	for (int i = 0; i < variables.size(); i++) {
		if (variables[i].type == type) {
			std::cout << variables[i].name;
			std::cout << show_footer_variables(type, i) << std::endl;
		}
	}
}

// name - название переменной.
// Если переменная объявлена в системе, то возвращает её индекс, иначе вернёт -1.
int Variables_stack::is_variable(std::string name) {
	for (int i = 0; i < variables.size(); i++) {
		if (variables[i].name == name) return i;
	}
	return -1;
}

// name - название переменной.
// value - значение переменной.
// Если переменной нет в системе, то добавляет её.
// Если есть и переменная не является константой или функцией, то обновляет её значение.
// В противном случае выбрасывает исключение.
// Возвращает индекс изменённой или добавленной переменной.
int Variables_stack::add_variable(std::string name, double value) {
	int index = is_variable(name);
	if (index != -1) {
		char type = variables[index].type;
		switch (type) {
		case VARIABLE:
			variables[index].value = value;
			return index;
		case CONSTANTS:
			throw ErrorConstantInitial{};
		case FUNCTIONS:
			throw ErrorFunctionInitial{};
		}
	}
	else {
		variables.push_back(Variables{ name, value, VARIABLE, "" });
		return variables.size() - 1;
	}
}

// name - название переменной.
// Возвращает значение переменной по её имени.
// Если переменная не определена, то вернёт false.
double Variables_stack::get_variable(std::string name) {
	for (Variables v : variables) {
		if (v.name == name)
			return v.value;
	}
	return false;
}

// index - индекс переменной в стеке переменных.
// Возвращает значение переменной по её индексу.
double Variables_stack::get_variable(int index) {
	return variables[index].value;
}

// index - индекс переменной в стеке переменных.
// Возвращает имя переменной по её индексу.
std::string Variables_stack::get_name(int index) {
	return variables[index].name;
}

// name - название переменной.
// Возвращает тип переменной по её имени.
// Если переменная не определена, то вернёт false.
char Variables_stack::get_type(std::string name) {
	for (Variables v : variables) {
		if (v.name == name)
			return v.type;
	}
	return false;
}

// ch - символ Римской цифры.
// Добавляет один символ Римской цифры в переменную символов.
void Variables_stack::add_roman(char ch) {
	roman += ch;
}

// Возвращает Римскую цифру.
const std::string Variables_stack::get_roman() {
	return roman;
}

// Вырезает последнюю Римскую цифру.
// Возвращает вырезанную цифру.
char Variables_stack::roman_decrement() {
	char last_number = roman[roman.size() - 1];
	roman.pop_back();
	return last_number;
}

// Обнуляет Римскую цифру.
void Variables_stack::reset_roman() {
	roman = "";
}