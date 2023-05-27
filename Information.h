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
	const std::string separator = "||||||||||";	// Строка в файле, означающая конец совета.
	std::string path = "files/tips.txt";
public:
	// Конструктор читает файл с советами и записывает советы в массив tips.
	// Если файл не найден, то выведет сообщение, что файл не найден.
	// Конструктор не выбрасывает исключение, потому что почемуто я не могу его обработать. Пока не поянтно почему.
	Information() {
		setlocale(LC_ALL, "russian");
		std::string tip;
		std::ifstream istr(path);
		if (!istr.is_open()) {
			std::cerr << "Файл \"" << path << "\" не может быть открыт!" << std::endl;
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

	// Возвращает случайную строку с информацией из массива tips.
	std::string get_tip() {
		const int min = 0;
		const int max = tips.size() - 1;
		if (max <= 0) throw ErrorEmptyFile{};
		srand(time(NULL));
		int r = min + rand() % (max - min + 1);
		return tips[r];
	}
};