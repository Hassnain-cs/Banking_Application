#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include "validation.h"

#define INPUT_BUFFER_SIZE 256


   // This helper function will:
   // - Reads one full line safely using fgets().
   // - Prevents scanf-related crashes/problems
   // - Handles long input
   // - Clears extra characters from input buffer if user types too much

static int readLine(char* buffer, size_t size)
{
    if (fgets(buffer, (int)size, stdin) == NULL)
    {
        return 0; // input error
    }

    // Check if newline exists in the buffer
    size_t len = strlen(buffer);

    if (len > 0 && buffer[len - 1] == '\n')
    {
        // Remove newline
        buffer[len - 1] = '\0';
    }
    else
    {
        // Input was too long, clear remaining characters from stdin
        int ch;
        while ((ch = getchar()) != '\n' && ch != EOF)
        {
            // discard extra characters
        }
    }

    return 1;
}


    // this safe integer input will: 
    // - Rejects letters/symbols
    // - Rejects empty input
    // - Uses strtol for reliable conversion

int getIntInput(const char* prompt)
{
    char buffer[INPUT_BUFFER_SIZE];
    char* endPtr;
    long value;

    while (1)
    {
        printf("%s", prompt);

        if (!readLine(buffer, sizeof(buffer)))
        {
            printf("Input error. Please try again.\n");
            continue;
        }

        // Reject empty input
        if (buffer[0] == '\0')
        {
            printf("Input cannot be empty. Please enter an integer.\n");
            continue;
        }

        errno = 0;
        value = strtol(buffer, &endPtr, 10);

        // This will check:
        // 1. no digits found
        // 2. extra characters exist after number
        // 3. overflow/underflow happened
        if (endPtr == buffer || *endPtr != '\0' || errno != 0)
        {
            printf("Invalid input. Please enter a valid integer.\n");
            continue;
        }

        return (int)value;
    }
}


   // This integer input with range checking will:
   // - Reuses getIntInput()
   // - Good for menus and bounded choices

int getIntInRange(const char* prompt, int min, int max)
{
    int value;

    while (1)
    {
        value = getIntInput(prompt);

        if (value < min || value > max)
        {
            printf("Please enter a number between %d and %d.\n", min, max);
            continue;
        }

        return value;
    }
}


   // The safe float input are:
   // - Rejects letters/symbols
   // - Uses strtof for reliable float conversion

float getFloatInput(const char* prompt)
{
    char buffer[INPUT_BUFFER_SIZE];
    char* endPtr;
    float value;

    while (1)
    {
        printf("%s", prompt);

        if (!readLine(buffer, sizeof(buffer)))
        {
            printf("Input error. Please try again.\n");
            continue;
        }

        // Reject empty input
        if (buffer[0] == '\0')
        {
            printf("Input cannot be empty. Please enter a number.\n");
            continue;
        }

        errno = 0;
        value = strtof(buffer, &endPtr);

        // Check:
        // 1. no digits found
        // 2. extra characters exist after number
        // 3. conversion error happened
        if (endPtr == buffer || *endPtr != '\0' || errno != 0)
        {
            printf("Invalid input. Please enter a valid number.\n");
            continue;
        }

        return value;
    }
}

   // This float input with range checking will:
   // - Useful for amounts like deposits/transfers
float getFloatInRange(const char* prompt, float min, float max)
{
    float value;

    while (1)
    {
        value = getFloatInput(prompt);

        if (value < min || value > max)
        {
            printf("Please enter a value between %.2f and %.2f.\n", min, max);
            continue;
        }

        return value;
    }
}


   // This Safe string input will:
   // - Read full line safely
   // - Prevent buffer overflow
   // - Handle long input by trimming and clearing stdin

void getStringInput(const char* prompt, char* output, size_t size)
{
    while (1)
    {
        printf("%s", prompt);

        if (!readLine(output, size))
        {
            printf("Input error. Please try again.\n");
            continue;
        }

        if (output[0] == '\0')
        {
            printf("Input cannot be empty. Please try again.\n");
            continue;
        }

        return;
    }
}