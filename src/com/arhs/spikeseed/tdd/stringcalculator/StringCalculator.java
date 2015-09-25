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
     * Requirement 6: Support different delimiters
     * To change a delimiter, the beginning of the string will contain a separate line that looks like this:
     * “//[delimiter]\n[numbers…]” for example “//;\n1;2” should take 1 and 2 as parameters and return 3
     * where the default delimiter is ‘;’ .
     *
     * @param numbers
     */
    public static int add(final String numbers) {
        String delimiter = ",|n";
        String numbersWithoutDelimiter = numbers;
        if (numbers.startsWith("//")) {
            int delimiterIndex = numbers.indexOf("//") + 2;
            delimiter = numbers.substring(delimiterIndex, delimiterIndex + 1);
            numbersWithoutDelimiter = numbers.substring(numbers.indexOf("n") + 1);
        }
        return add(numbersWithoutDelimiter, delimiter);
    }

    private static int add(final String numbers, final String delimiter) {
        int returnValue = 0;
        String[] numbersArray = numbers.split(delimiter);
        for (String number : numbersArray) {
            if (!number.trim().isEmpty()) {
                returnValue += Integer.parseInt(number.trim());
            }
        }
        return returnValue;
    }
}
