#include <StringCalculator.h>

#include <gmock/gmock.h>
#include <exception>

// Create a simple String calculator with a method int Add(string numbers)
// Requirement 1: The method can take 0, 1 or 2 numbers, and will return their sum (for an empty string it will return 0) for example “” or “1” or “1,2”


TEST(StringCalculatorTest, when2NumbersAreUsedThenNoExceptionIsThrown){
	ASSERT_NO_THROW(StringCalculator::add("1,2"));
};

TEST(StringCalculatorTest, whenNonNumberIsUsedThenExceptionIsThrown){
	ASSERT_THROW(StringCalculator::add("1,X"), std::runtime_error);
};

// Requirement 2: For an empty string the method will return 0
TEST(StringCalculatorTest, whenEmptyStringIsUsedThenReturnValueIs0){
	ASSERT_EQ(0, StringCalculator::add(""));
};

// Requirement 3: Method will return their sum of number
TEST(StringCalculatorTest, whenOneNumberIsUsedThenReturnValueIsThatSameNumber){
	ASSERT_EQ(3, StringCalculator::add("3"));
};

TEST(StringCalculatorTest, whenTwoNumbersAreUsedThenReturnValueIsTheirSum){
	ASSERT_EQ(3+6, StringCalculator::add("3,6"));
};

// Requirement 4: Allow the Add method to handle an unknown amount of numbers
TEST(StringCalculatorTest, whenAnyNumberOfNumbersIsUsedThenReturnValuesAreTheirSums){
	ASSERT_EQ(3+6+15+18+46+33, StringCalculator::add("3,6,15,18,46,33"));
};

// Requirement 5: Allow the Add method to handle new lines between numbers (instead of commas)
TEST(StringCalculatorTest, whenNewLineIsUsedBetweenNumbersThenReturnValuesAreTheirSums){
	ASSERT_EQ(3+6+15, StringCalculator::add("3,6n15"));
};

// Requirement 6: Support different delimiters
TEST(StringCalculatorTest, whenDelimiterIsSpecifiedThenItIsUsedToSeparateNumbers){
	ASSERT_EQ(3+6+15, StringCalculator::add("//;n3;6;15"));
};

TEST(StringCalculatorTest, whenNegativeNumberIsUsedThenRuntimeExceptionIsThrown){
	ASSERT_THROW(StringCalculator::add("3,-6,15,18,46,33"), std::runtime_error);
};

// Requirement 7: Negative numbers will throw an exception
TEST(StringCalculatorTest, whenNegativeNumbersAreUsedThenRuntimeExceptionIsThrown){
	try{
		StringCalculator::add("3,-6,15,-18,46,33");
	}
	catch(std::runtime_error e){
		ASSERT_EQ("Negatives not allowed: [-6, -18]", std::string(e.what()));
	}
};

// Requirement 8: Numbers bigger than 1000 should be ignored
TEST(StringCalculatorTest, whenOneOrMoreNumbersAreGreaterThan1000IsUsedThenItIsNotIncludedInSum){
	ASSERT_EQ(3+1000+6, StringCalculator::add("3,1000,1001,6,1234"));
};
