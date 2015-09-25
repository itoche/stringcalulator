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
     * Requirement 4: Allow the Add method to handle an unknown amount of numbers
     * Requirement 5: Allow the Add method to handle new lines between numbers (instead of commas)
     *
     * @param numbers
     */
    public static int add(final String numbers) {
        int returnValue = 0;
        String[] numbersArray = numbers.split(",|n"); // Added |n to the split regex
        for (String number : numbersArray) {
            if (!number.trim().isEmpty()) {
                returnValue += Integer.parseInt(number.trim());
            }
        }
        return returnValue;
    }
}
