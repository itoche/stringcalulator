#include "StringCalculator.h"
#include <StringUtils.h>

#include <vector>
#include <sstream>

int StringCalculator::add(const std::string& numbers) {
	std::string delimiter = ",n";
	std::string numbersWithoutDelimiter(numbers);
	if (numbers.substr(0,2) == "//") {
		size_t delimiterIndex = numbers.find("//") + 2;
		delimiter = numbers.substr(delimiterIndex, 1);
		numbersWithoutDelimiter = numbers.substr(numbers.find("n") + 1);
	}
	return add(numbersWithoutDelimiter, delimiter);
}

int StringCalculator::add(const std::string& numbers,
		const std::string& delimiter) {
	int returnValue = 0;
	std::vector<std::string> numbersArray;
	tokenize(numbers, numbersArray, delimiter);
	std::vector<int> negativeNumbers;;
	for (const std::string& number : numbersArray) {
		if (!trim(number).empty()) {
			try{
				int numberInt = std::stoi(trim(number));
				if (numberInt < 0) {
					negativeNumbers.push_back(numberInt);
				} else if (numberInt <= 1000) {
					returnValue += numberInt;
				}
			}
			catch(...){
				std::ostringstream oss;
				oss << "Cannot parse " << number << " as a number";
				throw std::runtime_error(oss.str());
			}

		}
	}
	if (negativeNumbers.size() > 0) {
		std::ostringstream oss;
		oss << "Negatives not allowed: [";
		for(size_t index=0;index<negativeNumbers.size();index++){
			int number = negativeNumbers[index];
			oss << number;
			if(index != (negativeNumbers.size()-1)){
				oss << ", ";
			}
		}
		oss << "]";
		throw std::runtime_error(oss.str());
	}
	return returnValue;
}
