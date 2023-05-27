#pragma once
#include <fstream>
#include <string>
#include <vector>
#include <stdlib.h>
#include <time.h>
#include "errors.h"

class Information
{
	std::vector<std::string> tips;
	const std::string separator = "||||||||||";	// ������ � �����, ���������� ����� ������.
	std::string path = "files/tips.txt";
public:
	// ����������� ������ ���� � �������� � ���������� ������ � ������ tips.
	// ���� ���� �� ������, �� ������� ���������, ��� ���� �� ������.
	// ����������� �� ����������� ����������, ������ ��� �������� � �� ���� ��� ����������. ���� �� ������� ������.
	Information() {
		setlocale(LC_ALL, "russian");
		std::string tip;
		std::ifstream istr(path);
		if (!istr.is_open()) {
			std::cerr << "���� \"" << path << "\" �� ����� ���� ������!" << std::endl;
			return;
		}

		while (!istr.eof()) {
			std::string temp;
			getline(istr, temp);
			if (temp == separator) {
				tips.push_back(tip);
				tip = "";
				continue;
			}
			tip += temp + '\n';
		}
		istr.close();
	}
	~Information() {};

	// ���������� ��������� ������ � ����������� �� ������� tips.
	std::string get_tip() {
		const int min = 0;
		const int max = tips.size() - 1;
		if (max <= 0) throw ErrorEmptyFile{};
		srand(time(NULL));
		int r = min + rand() % (max - min + 1);
		return tips[r];
	}
};