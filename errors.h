#pragma once

// ОБЩИЕ ОШИБКИ
class ErrorInvalidInput {};			// Неверный ввод выражения.
class ErrorUnknownToken {};			// Неизвестная лексема.
class ErrorMissingLeftBracket {};	// Пропущенная открывающая скобка.
class ErrorMissingRightBracket {};	// Пропущенная закрывающая скобка.
class ErrorNoLeftExpression {};		// Нет начального выражения.
class ErrorUseNext {};				// Неверный ввод выражения. Чтобы использовать несколько выражений в одной строке используйти запятую между выражениями.
class ErrorNoMultiExpression {};	// Выполнить в одной строке можно только одно выражение, но в одной строке можно присвоить значения нескольким переменным.

// ОШИБКИ ПЕРЕМЕННЫХ, КОНСТАНТ И ФУНКЦИЙ
class ErrorNoVariable {};			// Переменная не определена.
class ErrorDefinedVariables {};		// Невозможно отобразить определённые в программе переменные.
class ErrorConstantInitial {};		// Нельзя присвоить значение константе. Данное имя используется системой.
class ErrorFunctionInitial {};		// Нельзя присвоить значение функции. Данное имя используется системой.
class ErrorIvalidEqually {};		// Не верное присваивание.

// ОШИБКИ ЧИСЕЛ
class ErrorDivisionZero {};			// Деление на ноль.
class ErrorDigitalNotEqually {};	// Число не может хранить значение. Присвойте значение переменной.
class ErrorNegative {};				// Отрицательное число.
class ErrorOverflowValue {};		// Переполнение значения факториала.
class ErrorBadRomanInput {};		// Неверный ввод Римских цифр.
class ErrorRomanNotCorrect {};		// Функция roman ожидает либо Римские либо Арабские цифры.
class ErrorNotArgumentFunction {};	// Функция ожидает аргумента. Пропущен аргумент функции.

// ОШИБКИ СРАВНЕНИЯ
class ErrorNotComparison {};		// Сравнить можно только два числа или две переменных. Результат сравнения не может быть присвоен переменной.

// ОШИБКИ РАБОТЫ С ФАЙЛАМИ
class ErrorNotFile {};				// Файл не найден.
class ErrorEmptyFile {};			// Файл пустой.