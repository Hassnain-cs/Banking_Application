#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>


int getIntInRange(const char* prompt, int min, int max) {
    char line[100];   
    int value;

    while (1) {
        printf("%s", prompt);

        if (!fgets(line, sizeof(line), stdin)) {
            continue;
        }

        if (sscanf(line, "%d", &value) != 1) {
            printf("Invalid input. Please enter a number.\n");
            continue;
        }

        if (value < min || value > max) {
            printf("Please enter a number between %d and %d.\n", min, max);
            continue;
        }

        return value;
    }
}