#pragma once

class ErrorDivisionZero {};			// ������� �� ����.
class ErrorUnknownToken {};			// ����������� �������.
class ErrorMissingRightBracket {};	// ����������� ����������� ������.
class ErrorMissingLeftBracket {};	// ����������� ����������� ������.
class ErrorInvalidInput {};			// �������� ����.
class ErrorNegativeFactorial {};	// ������������� ���������.
class ErrorOverflowValue {};		// ������������ �������� ����������.
class ErrorNoLeftExpression {};		// ��� ���������� ���������.
class ErrorNoVariable {};			// ���������� �� ����������.
class ErrorNumberVariable {};		// �������� ���������� �� ����� ��������� �����.
class ErrorNotComparison {};		// �������� ����� ������ ��� ����� ��� ��� ����������. ��������� ��������� �� ����� ���� �������� ����������.
class ErrorConstantInitial {};		// ������ ��������� �������� ���������.
class ErrorNotLetter {};			// ������ ������������ ������ ����� ��� �������� ����������.