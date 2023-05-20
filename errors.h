#pragma once

class ErrorInvalidInput {};			// Неверный ввод выражения.
class ErrorDefinedVariables {};		// Невозможно отобразить определённые в программе переменные.
class ErrorConstantInitial {};		// Нельзя присвоить значение константе. Данное имя используется системой.
class ErrorFunctionInitial {};		// Нельзя присвоить значение функции. Данное имя используется системой.
class ErrorDivisionZero {};			// Деление на ноль.
class ErrorUnknownToken {};			// Неизвестная лексема.
class ErrorMissingLeftBracket {};	// Пропущенная открывающая скобка.
class ErrorNotComparison {};		// Сравнить можно только два числа или две переменных. Результат сравнения не может быть присвоен переменной.
class ErrorDigitalNotEqually {};	// Число не может хранить значение. Присвойте значение переменной.
class ErrorNoVariable {};			// Переменная не определена.
class ErrorMissingRightBracket {};	// Пропущенная закрывающая скобка.
class ErrorNoLeftExpression {};		// Нет начального выражения.
class ErrorUseNext {};				// Неверный ввод выражения. Чтобы использовать несколько выражений в одной строке используйти запятую между выражениями.
class ErrorNegativeFactorial {};	// Отрицательный факториал.
class ErrorOverflowValue {};		// Переполнение значения факториала.
class ErrorNoMultiExpression {};	// Выполнить в одной строке можно только одно выражение, но в одной строке можно присвоить значения нескольким переменным.
class ErrorIvalidEqually {};		// Не верное присваивание.