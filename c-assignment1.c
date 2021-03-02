/**
 * @file c-assignment1.c
 * @author Viviana Nordvall (viviana.apollon-nordvall@yaelev.se)
 * @brief Make a program to read personal identity numbers (personnummer) in the format ofYYMMDD-nnnC​ from the terminal 
 * and check if the personnummer is correct or not.
 * @version 0.1
 * @date 2021-02-28
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>

#define PERSONAL_NUMBER "YYMMDD-nnnC"
#define YEAR_MAX 99U
#define MONTH_MIN 1U
#define MONTH_MAX 12U
#define DAY_MIN 1U
#define DAY_MAX 31U
#define SERIAL_NUM_MAX 999U
#define CONTROL_NUM_MAX 9U
#define FEBRUARY 2U

int main(void)
{
    char format[] = PERSONAL_NUMBER;
    char string[sizeof(PERSONAL_NUMBER) + 1] = {0}; // An array of a string must always be at least one element longer, than string length to
                                                    // accomodate null character and lenght strength is initialized to 0

    // Read the string from the standard input
    printf("Please enter personal identity number in the format of %s: ", PERSONAL_NUMBER);
    fgets(string, sizeof(string), stdin);
    string[strlen(string) - 1] = '\0'; // fgets captures the newline also and we don't want to have it in the string

    int i = 0;
    bool is_valid = true;

    while (i < sizeof(PERSONAL_NUMBER) - 1)
    {
        if (isalpha(format[i]))
        {
            if (!isdigit(string[i]))
            {
                is_valid = false;
                break;
            }
        }
        else
        {
            if (format[i] != string[i])
            {
                is_valid = false;
                break;
            }
        }
        i++;
    }

    if (is_valid)
    {
        unsigned int year = 0, month = 0, day = 0, serial_num = 0, control_num = 0;
        sscanf(string, "%02u%02u%02u-%03u%1u", &year, &month, &day, &serial_num, &control_num);
        if ((year > YEAR_MAX) || (month < MONTH_MIN) || (month > MONTH_MAX) || (day < DAY_MIN) || (day > DAY_MAX) ||
            (serial_num > SERIAL_NUM_MAX) || (control_num > CONTROL_NUM_MAX))
        {
            is_valid = false;
        }
        else
        {
            uint8_t days[MONTH_MAX] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
            if (day > days[month - 1])
            {
                is_valid = false;
                if (month == FEBRUARY)
                {
                    if ((year % 400 == 0) || (year % 4 == 0) && (year % 100 != 0))
                    {
                        if (day == days[month - 1] + 1)
                        {
                            is_valid = true;
                        }
                    }
                }
            }
        }
    }
    if (is_valid)
    {
        int sum;
        int digit_C; //integer to check the control number
        //The string are numbers. And have a certain number in the ASCII table.
        //E.g string 0 returns value 48. By subtracting 48 i get the literal number that was inserted and not the value in ASCII.

        int num0 = 2 * (string[0] - 48U); // Starting with the first digit double the value of every second digit
        int num2 = 2 * (string[2] - 48U);
        int num4 = 2 * (string[4] - 48U);
        int num7 = 2 * (string[7] - 48U);
        int num9 = 2 * (string[9] - 48U);

        // If doubling of a number results in a two digits number, the integer minus 9 to get a single digit number.
        if (num0 > 9)
        {
            num0 = num0 - 9;
        }
        if (num2 > 9)
        {
            num2 = num2 - 9;
        }
        if (num4 > 9)
        {
            num4 = num4 - 9;
        }
        if (num7 > 9)
        {
            num7 = num7 - 9;
        }
        if (num9 > 9)
        {
            num9 = num9 - 9;
        }
        sum = num0 + num2 + num4 + num7 + num9 + (string[1] - 48U) + (string[3] - 48U) + (string[5] - 48U) + (string[8] - 48U); //sum of digits
        //printf("sum is %d\n", sum); // Used to check if the sum calculated is correct.

        digit_C = (10 - (sum % 10)) % 10; // if the total modulo 10 is equal to 0  then the number is valid according to the Luhn formula
        //printf("Last digit should be %d,\n", digit_C); // Used to check if the control digit is correct.

        if (digit_C != string[10] - 48U)
        {
            is_valid = false;
        }
        else
        {
            is_valid = true;
        }
    }
    printf("The format of the entered personal identity number is%s valid!\n", is_valid ? "" : " not");

    return 0;
}
