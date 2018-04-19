#ifndef CREADER_H
#define CREADER_h
#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include <iterator>
#include <sstream>


class CReader {
public:
	std::string file_name;
	int num_string = 0;
	std::vector<std::string> first_number;
	std::vector<std::string> second_number;
	std::vector<std::string> result_number;
	CReader(std:: string name){
		file_name = name;
		std::ifstream reader(file_name);
		int i = 0;
		std::string str;
		while (std::getline(reader, str) && i < 100) {
			num_string++;
			std::istringstream iss(str);
			std::vector<std::string> results(std::istream_iterator<std::string>{iss},
				std::istream_iterator<std::string>());
			first_number.emplace_back(results[0]);
			second_number.emplace_back(results[1]);
			result_number.emplace_back(results[2]);
			i++;
		}
	}

	~CReader(){
		first_number.clear();
		second_number.clear();
		result_number.clear();
	}
};

#endif CREADER_H