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