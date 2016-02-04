#pragma once

#include <string>

class StringCalculator{
public:
	static int add(const std::string & numbers);
	static int add(const std::string & numbers, const std::string & delimiter);
};
