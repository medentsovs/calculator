#pragma once

// ����� ������
class ErrorInvalidInput {};			// �������� ���� ���������.
class ErrorUnknownToken {};			// ����������� �������.
class ErrorMissingLeftBracket {};	// ����������� ����������� ������.
class ErrorMissingRightBracket {};	// ����������� ����������� ������.
class ErrorNoLeftExpression {};		// ��� ���������� ���������.
class ErrorUseNext {};				// �������� ���� ���������. ����� ������������ ��������� ��������� � ����� ������ ����������� ������� ����� �����������.
class ErrorNoMultiExpression {};	// ��������� � ����� ������ ����� ������ ���� ���������, �� � ����� ������ ����� ��������� �������� ���������� ����������.

// ������ ����������, �������� � �������
class ErrorNoVariable {};			// ���������� �� ����������.
class ErrorDefinedVariables {};		// ���������� ���������� ����������� � ��������� ����������.
class ErrorConstantInitial {};		// ������ ��������� �������� ���������. ������ ��� ������������ ��������.
class ErrorFunctionInitial {};		// ������ ��������� �������� �������. ������ ��� ������������ ��������.
class ErrorIvalidEqually {};		// �� ������ ������������.

// ������ �����
class ErrorDivisionZero {};			// ������� �� ����.
class ErrorDigitalNotEqually {};	// ����� �� ����� ������� ��������. ��������� �������� ����������.
class ErrorNegative {};				// ������������� �����.
class ErrorOverflowValue {};		// ������������ �������� ����������.
class ErrorBadRomanInput {};		// �������� ���� ������� ����.
class ErrorRomanNotCorrect {};		// ������� roman ������� ���� ������� ���� �������� �����.
class ErrorNotArgumentFunction {};	// ������� ������� ���������. �������� �������� �������.

// ������ ���������
class ErrorNotComparison {};		// �������� ����� ������ ��� ����� ��� ��� ����������. ��������� ��������� �� ����� ���� �������� ����������.

// ������ ������ � �������
class ErrorNotFile {};				// ���� �� ������.
class ErrorEmptyFile {};			// ���� ������.