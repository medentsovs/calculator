#pragma once

// ������ ����������.
class Variables
{
public:
	// �������� ����������
	std::string name = "";
	// �������� ����������
	double value;
	// ��� ����������: [V]ariable | [C]onstat | [F]unction
	char type;
	// �������� ����������. ��� ������ ����� ��� �������.
	std::string comment;
};