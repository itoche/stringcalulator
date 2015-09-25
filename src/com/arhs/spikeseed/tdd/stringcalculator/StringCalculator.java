package com.arhs.spikeseed.tdd.stringcalculator;

/**
 * Created by arhs on 25/09/15.
 */
public class StringCalculator {

    /**
     * Create a simple String calculator with a method int Add(string numbers)
     * Requirement 1: The method can take 0, 1 or 2 numbers, and will return their sum (for an empty string it will return 0) for example “” or “1” or “1,2”
     * Requirement 2: For an empty string the method will return 0
     * Requirement 3: Method will return their sum of number
     *
     * @param numbers
     */
    public static final int add(final String numbers) { // Changed void to int
        int returnValue = 0;
        String[] numbersArray = numbers.split(",");
        if (numbersArray.length > 2) {
            throw new RuntimeException("Up to 2 numbers separated by comma (,) are allowed");
        }
        for (String number : numbersArray) {
            if (!number.trim().isEmpty()) { // After refactoring
                returnValue += Integer.parseInt(number);
            }
        }
        return returnValue;
    }
}
