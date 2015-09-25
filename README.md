# stringcalulator
StringCalculator (http://technologyconversations.com/2013/12/20/test-driven-development-tdd-example-walkthrough/)

Create a simple String calculator with a method int Add(string numbers)

Requirement 1: The method can take 0, 1 or 2 numbers, and will return their sum (for an empty string it will return 0) for example “” or “1” or “1,2”
Requirement 2: For an empty string the method will return 0
Requirement 3: Method will return their sum of number
Requirement 4: Allow the Add method to handle an unknown amount of numbers
Requirement 5: Allow the Add method to handle new lines between numbers (instead of commas)
Requirement 6: Support different delimiters
To change a delimiter, the beginning of the string will contain a separate line that looks like this:
“//[delimiter]\n[numbers…]” for example “//;\n1;2” should take 1 and 2 as parameters and return 3
where the default delimiter is ‘;’ .
The first line is optional. All existing scenarios should still be supported
Requirement 7: Negative numbers will throw an exception
Calling Add with a negative number will throw an exception “negatives not allowed” – and the negative that was passed.
If there are multiple negatives, show all of them in the exception message.
Requirement 8: Numbers bigger than 1000 should be ignored
