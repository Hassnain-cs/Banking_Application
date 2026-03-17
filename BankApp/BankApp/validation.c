
#include <stdio.h>   
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <errno.h>

#include "validation.h"

static void trimNewline(char* s) {
    if (!s) return;
    size_t len = strlen(s);
    while (len > 0 && (s[len - 1] == '\n' || s[len - 1] == '\r')) {
        s[len - 1] = '\0';
        len--;
    }
}

void readLine(const char* prompt, char* buffer, int bufferSize) {
    if (!buffer || bufferSize <= 0) return;

    if (prompt) printf("%s", prompt);

    if (!fgets(buffer, bufferSize, stdin)) {
        buffer[0] = '\0';
        return;
    }

    trimNewline(buffer);

    if ((int)strlen(buffer) == bufferSize - 1) {
        int c;
        while ((c = getchar()) != '\n' && c != EOF) {}
    }
}

int getIntInRange(const char* prompt, int min, int max) {
    char line[128];

    for (;;) {
        readLine(prompt, line, (int)sizeof(line));

        if (line[0] == '\0') {
            printf("Invalid input. Try again.\n");
            continue;
        }

        char* endPtr = NULL;
        errno = 0;
        long value = strtol(line, &endPtr, 10);

        if (errno != 0 || endPtr == line || *endPtr != '\0') {
            printf("Invalid number. Try again.\n");
            continue;
        }

        if (value < min || value > max || value < INT_MIN || value > INT_MAX) {
            printf("Number must be between %d and %d.\n", min, max);
            continue;
        }

        return (int)value;
    }
}
