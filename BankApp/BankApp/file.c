#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "file.h"
#include "account.h"

static const char* ACCOUNTS_FILE = "accounts.txt";

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
        strncpy_s(a.address, sizeof(a.address), tok, _TRUNCATE);

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

/*
Saves all account data into file in structured format
*/
void saveAccountsToFile(void) {

    FILE* f = fopen(ACCOUNTS_FILE, "w");
    if (!f) return;

    for (int i = 0; i < accountCount; i++) {

        const Account* a = &accounts[i];

        fprintf(f,
            "%d|%s|%s|%d|%d|%d|%s|%s|%.2f|%d\n",
            a->accountNumber,
            a->firstName,
            a->lastName,
            a->birthDay,
            a->birthMonth,
            a->birthYear,
            a->address,
            a->password,
            a->balance,
            a->isActive
        );
    }

    fclose(f);
}

void loadTransactionsFromFile(void) {}
void saveTransactionsToFile(void) {}
void loadContactsFromFile(void) {}
void saveContactsToFile(void) {}