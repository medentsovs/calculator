#pragma once

class ErrorDivisionZero {};			// Деление на ноль.
class ErrorUnknownToken {};			// Неизвестная лексема.
class ErrorMissingRightBracket {};	// Пропущенная закрывающая скобка.
class ErrorMissingLeftBracket {};	// Пропущенная открывающая скобка.
class ErrorInvalidInput {};			// Неверный ввод.
class ErrorNegativeFactorial {};	// Отрицательный факториал.
class ErrorOverflowValue {};		// Переполнение значения факториала.
class ErrorNoLeftExpression {};		// Нет начального выражения.
class ErrorNoVariable {};			// Переменная не определена.
class ErrorNumberVariable {};		// Название переменной не может содержать цифры.
class ErrorNotComparison {};		// Сравнить можно только два числа или две переменных. Результат сравнения не может быть присвоен переменной.
class ErrorConstantInitial {};		// Нельзя присвоить значение константе.
class ErrorNotLetter {};			// Нельзя использовать данную букву для названия переменной.