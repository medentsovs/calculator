#pragma once

const std::string   TITLE = "Calculator";
const std::string VERSION = "0.7.1";

// Константы, которые представляют сами себя: + - * / %  ( ) { }
const char         NUMBER = 'N';	// Константа используется программой и используется для обозначения цифры в стеке лексем.
const char       VARIABLE = 'V';	// Константа используется программой и используется для обозначения названия переменной.
const char      EXHIBITOR = 'e';	// Константа используется программой для определения числа в экспоненциальном виде: 4.845e+03.
const std::string   INPUT = ">> ";	// Приглашение пользователя к вводу.
const std::string  RESULT = "= ";	// Вывод ответа выражения.

const char        INITIAL = '=';	// Инициализация переменной.
const char       SHOWVARS = 'V';	// Просмотр определённых переменных.
const char      FACTORIAL = '!';	// Факториал
const char           EXIT = '#';	// Выход