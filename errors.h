#pragma once

// Ошибка деления на ноль.
class ErrorDivisionZero {};
// Ошибка неизвестной лексемы.
class ErrorUnknownToken {};
// Ошибка пропущенной закрывающей скобки.
class ErrorMissingRightBracket {};
// Ошибка пропущенной открывающей скобки.
class ErrorMissingLeftBracket {};
// Ошибка неверного ввода.
class ErrorInvalidInput {};
// Ошибка отрицательного факториала.
class ErrorNegativeFactorial {};
// Ошибка переполнения значения факториала.
class ErrorOverflowValue {};
// Ошибка нет начального выражения.
class ErrorNoLeftExpression {};
// Ошибка переменная не определена.
class ErrorNoVariable {};
