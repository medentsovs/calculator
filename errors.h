#pragma once

class ErrorInvalidInput {};			// �������� ���� ���������.
class ErrorDefinedVariables {};		// ���������� ���������� ����������� � ��������� ����������.
class ErrorConstantInitial {};		// ������ ��������� �������� ���������. ������ ��� ������������ ��������.
class ErrorFunctionInitial {};		// ������ ��������� �������� �������. ������ ��� ������������ ��������.
class ErrorDivisionZero {};			// ������� �� ����.
class ErrorUnknownToken {};			// ����������� �������.
class ErrorMissingLeftBracket {};	// ����������� ����������� ������.
class ErrorNotComparison {};		// �������� ����� ������ ��� ����� ��� ��� ����������. ��������� ��������� �� ����� ���� �������� ����������.
class ErrorDigitalNotEqually {};	// ����� �� ����� ������� ��������. ��������� �������� ����������.
class ErrorNoVariable {};			// ���������� �� ����������.
class ErrorMissingRightBracket {};	// ����������� ����������� ������.
class ErrorNoLeftExpression {};		// ��� ���������� ���������.
class ErrorUseNext {};				// �������� ���� ���������. ����� ������������ ��������� ��������� � ����� ������ ����������� ������� ����� �����������.
class ErrorNegativeFactorial {};	// ������������� ���������.
class ErrorOverflowValue {};		// ������������ �������� ����������.
class ErrorNoMultiExpression {};	// ��������� � ����� ������ ����� ������ ���� ���������, �� � ����� ������ ����� ��������� �������� ���������� ����������.
class ErrorIvalidEqually {};		// �� ������ ������������.