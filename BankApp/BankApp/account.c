#include <stdio.h>
#include <string.h>
#include <limits.h>
#include "account.h"
#include "validation.h"

Account g_accounts[MAX_ACCOUNTS];
int g_accountCount = 0;

static int g_sessionAccNum = -1;

int getCurrentSessionAccountNumber(void) { return g_sessionAccNum; }
void logout(void) { g_sessionAccNum = -1; }

Account* findAccountByNumber(int accountNumber) {
    for (int i = 0; i < g_accountCount; i++)
        if (g_accounts[i].accountNumber == accountNumber) return &g_accounts[i];
    return NULL;
}

static int addAccount(const Account* a) {
    if (g_accountCount >= MAX_ACCOUNTS) return 0;
    if (findAccountByNumber(a->accountNumber)) return 0;
    g_accounts[g_accountCount++] = *a;
    return 1;
}

void createAccount(void) {
    Account a = { 0 };
    printf("\n--- Create Account ---\n");
    readLine("Enter name: ", a.name, (int)sizeof(a.name));
    readLine("Password: ", a.password, (int)sizeof(a.password));

    // simple unique random-ish 6 digit
    int ok = 0;
    for (int tries = 0; tries < 1000 && !ok; tries++) {
        int cand = 100000 + (rand() % 900000);
        if (!findAccountByNumber(cand)) { a.accountNumber = cand; ok = 1; }
    }
    if (!ok) { printf("Failed to generate account number.\n"); return; }

    a.balance = 0.0;
    a.isActive = 1;

    if (!addAccount(&a)) { printf("Failed to create account.\n"); return; }
    printf("\nAccount created successfully! Your account number is: %d\n", a.accountNumber);
}

int login(void) {
    const int MAX_TRIES = 3;

    for (int attempt = 1; attempt <= MAX_TRIES; attempt++) {
        printf("\n--- Login (Attempt %d of %d) ---\n", attempt, MAX_TRIES);
        int accNum = getIntInRange("Enter account number: ", 1, INT_MAX);

        char pw[MAX_PASSWORD];
        readLine("Enter password: ", pw, (int)sizeof(pw));

        Account* acc = findAccountByNumber(accNum);
        if (!acc) { printf("Account not found.\n"); continue; }
        if (strcmp(acc->password, pw) != 0) { printf("Incorrect password. Please try again.\n"); continue; }

        g_sessionAccNum = acc->accountNumber;
        return acc->accountNumber;
    }

    printf("Too many failed attempts.\n");
    return -1;
}

int deleteAccountByNumber(int accountNumber) {
    int idx = -1;
    for (int i = 0; i < g_accountCount; i++) {
        if (g_accounts[i].accountNumber == accountNumber) { idx = i; break; }
    }
    if (idx == -1) return 0;
    for (int i = idx; i < g_accountCount - 1; i++) g_accounts[i] = g_accounts[i + 1];
    g_accountCount--;
    return 1;
}

int deleteCurrentAccount(void) {
    int accNum = getCurrentSessionAccountNumber();
    if (accNum == -1) { printf("No user is logged in.\n"); return 0; }

    Account* acc = findAccountByNumber(accNum);
    if (!acc) { printf("Session account not found.\n"); logout(); return 0; }

    char confirm[8];
    printf("\nWARNING: Delete account #%d (%s)\n", acc->accountNumber, acc->name);
    readLine("Type YES to confirm: ", confirm, (int)sizeof(confirm));
    if (strcmp(confirm, "YES") != 0) { printf("Delete cancelled.\n"); return 0; }

    if (!deleteAccountByNumber(accNum)) { printf("Delete failed.\n"); return 0; }

    logout();
    printf("Account deleted successfully.\n");
    return 1;
}
