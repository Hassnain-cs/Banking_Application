#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "file.h"
#include "account.h"



/*
Loads account data from file and reconstructs Account structures
*/
void loadAccountsFromFile(void) {

    FILE* f = fopen(ACCOUNTS_FILE, "r");
    if (!f) {
        return;
    }

    char line[512];

    while (fgets(line, sizeof(line), f)) {

        // Remove newline characters
        size_t len = strlen(line);
        while (len > 0 && (line[len - 1] == '\n' || line[len - 1] == '\r')) {
            line[len - 1] = '\0';
            len--;
        }

        if (line[0] == '\0') continue;

        Account a;
        memset(&a, 0, sizeof(a));

        char* ctx = NULL;

        // Parse data in correct order
        char* tok = strtok_s(line, "|", &ctx);
        if (!tok) continue;
        a.accountNumber = atoi(tok);

        tok = strtok_s(NULL, "|", &ctx);
        if (!tok) continue;
        strncpy_s(a.firstName, sizeof(a.firstName), tok, _TRUNCATE);

        tok = strtok_s(NULL, "|", &ctx);
        if (!tok) continue;
        strncpy_s(a.lastName, sizeof(a.lastName), tok, _TRUNCATE);

        tok = strtok_s(NULL, "|", &ctx);
        if (!tok) continue;
        a.birthDay = atoi(tok);

        tok = strtok_s(NULL, "|", &ctx);
        if (!tok) continue;
        a.birthMonth = atoi(tok);

        tok = strtok_s(NULL, "|", &ctx);
        if (!tok) continue;
        a.birthYear = atoi(tok);

        tok = strtok_s(NULL, "|", &ctx);
        if (!tok) continue;
        strncpy_s(a.streetNumber, sizeof(a.streetNumber), tok, _TRUNCATE);
        if (!tok) continue;
        strncpy_s(a.streetNumber, sizeof(a.streetNumber), tok, _TRUNCATE);

        tok = strtok_s(NULL, "|", &ctx);
        if (!tok) continue;
        strncpy_s(a.streetName, sizeof(a.streetName), tok, _TRUNCATE);

        tok = strtok_s(NULL, "|", &ctx);
        if (!tok) continue;
        strncpy_s(a.city, sizeof(a.city), tok, _TRUNCATE);

        tok = strtok_s(NULL, "|", &ctx);
        if (!tok) continue;
        strncpy_s(a.country, sizeof(a.country), tok, _TRUNCATE);

        tok = strtok_s(NULL, "|", &ctx);
        if (!tok) continue;
        strncpy_s(a.postalCode, sizeof(a.postalCode), tok, _TRUNCATE);

        tok = strtok_s(NULL, "|", &ctx);
        if (!tok) continue;
        strncpy_s(a.password, sizeof(a.password), tok, _TRUNCATE);

        tok = strtok_s(NULL, "|", &ctx);
        if (!tok) continue;
        a.balance = atof(tok);

        tok = strtok_s(NULL, "|", &ctx);
        if (!tok) continue;
        a.isActive = atoi(tok);

        addAccount(&a);
    }

    fclose(f);
}



void loadTransactionsFromFile(void) {}
void saveTransactionsToFile(void) {}
void loadContactsFromFile(void) {}
void saveContactsToFile(void) {}