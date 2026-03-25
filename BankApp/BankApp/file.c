#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "file.h"
#include "account.h"


static const char* ACCOUNTS_FILE = "accounts.txt";

void loadAccountsFromFile(void) {
    FILE* f = fopen(ACCOUNTS_FILE, "r");
    if (!f) {
        return;
    }

    char line[512];
    while (fgets(line, (int)sizeof(line), f)) {
        size_t len = strlen(line);
        while (len > 0 && (line[len - 1] == '\n' || line[len - 1] == '\r')) {
            line[len - 1] = '\0';
            len--;
        }

        if (line[0] == '\0') continue;

        Account a;
        memset(&a, 0, sizeof(a));

        // Tokenize by '|'
        char* ctx = NULL;
        char* tok = strtok_s(line, "|", &ctx);
        if (!tok) continue;
        a.accountNumber = atoi(tok);

        tok = strtok_s(NULL, "|", &ctx);
        if (!tok) continue;
        strncpy_s(a.name, sizeof(a.name), tok, _TRUNCATE);

        tok = strtok_s(NULL, "|", &ctx);
        if (!tok) continue;
        strncpy_s(a.password, sizeof(a.password), tok, _TRUNCATE);

        tok = strtok_s(NULL, "|", &ctx);
        if (!tok) continue;
        a.balance = atof(tok);

        tok = strtok_s(NULL, "|", &ctx);
        if (!tok) continue;
        a.isActive = atoi(tok);

        (void)addAccount(&a);
    }

    fclose(f);
}

void saveAccountsToFile(void) {
    FILE* f = fopen(ACCOUNTS_FILE, "w");
    if (!f) return;

    int i;
    for (i = 0; i < accountCount; i++) {
        const Account* a = &accounts[i];
        fprintf(f, "%d|%s|%s|%.2f|%d\n",
            a->accountNumber,
            a->name,
            a->password,
            a->balance,
            a->isActive);
    }

    fclose(f);
}

void loadTransactionsFromFile(void) {}
void saveTransactionsToFile(void) {}
void loadContactsFromFile(void) {}
void saveContactsToFile(void) {}
