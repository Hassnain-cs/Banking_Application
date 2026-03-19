#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <errno.h>
#include "validation.h"

static void trimNewline(char* s) {
    size_t n = strlen(s);
    while (n > 0 && (s[n - 1] == '\n' || s[n - 1] == '\r')) s[--n] = '\0';
}

void readLine(const char* prompt, char* buf, int sz) {
    if (prompt) printf("%s", prompt);
    if (!fgets(buf, sz, stdin)) { buf[0] = '\0'; return; }
    trimNewline(buf);
}

int getIntInRange(const char* prompt, int min, int max) {
    char line[128];
    for (;;) {
        readLine(prompt, line, (int)sizeof(line));
        char* end = 0; errno = 0;
        long v = strtol(line, &end, 10);
        if (errno || end == line || *end != '\0') { printf("Invalid number.\n"); continue; }
        if (v < min || v > max || v < INT_MIN || v > INT_MAX) {
            printf("Must be between %d and %d.\n", min, max);
            continue;
        }
        return (int)v;
    }
}
