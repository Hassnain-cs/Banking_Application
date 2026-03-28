#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include "account.h"
#include "model.h"

int accountCount = 0;
Account accounts[MAX_ACCOUNTS];

static int currentAccountIndex = -1;

static int generateAccountNumber(void);
static void readLine(const char* prompt, char* buffer, size_t size);
static int accountNumberExists(int accountNumber);
static int isStrongPassword(const char* password);

void createAccount(void) {
    if (accountCount >= MAX_ACCOUNTS) {
        printf("Maximum number of accounts reached.\n");
        return;
    }

    Account newAccount;
    newAccount.accountNumber = generateAccountNumber();
    newAccount.balance = 100.0; // default balance updated

    do {
        readLine("Enter account holder name: ", newAccount.name, sizeof(newAccount.name));
    } while (strlen(newAccount.name) == 0);

    char password1[50], password2[50];

    while (1) {
        readLine("Enter password: ", password1, sizeof(password1));
        readLine("Confirm password: ", password2, sizeof(password2));

        if (strcmp(password1, password2) != 0) {
            printf("Passwords do not match.\n");
            continue;
        }

        if (!isStrongPassword(password1)) {
            printf("Password must be at least 6 characters and contain letters and numbers.\n");
            continue;
        }

        break;
    }

    strcpy(newAccount.password, password1);
    newAccount.isActive = 1;

    accounts[accountCount++] = newAccount;

    printf("\nAccount created successfully.\n");
    printf("Account Number: %d\n", newAccount.accountNumber);
    printf("Starting Balance: $%.2f\n", newAccount.balance);
}

int login(void) {
    char line[100];
    int accNum;
    char password[50];

    printf("Enter account number: ");
    fgets(line, sizeof(line), stdin);
    sscanf(line, "%d", &accNum);

    readLine("Enter password: ", password, sizeof(password));

    for (int i = 0; i < accountCount; i++) {
        if (accounts[i].accountNumber == accNum && accounts[i].isActive) {
            if (strcmp(accounts[i].password, password) == 0) {
                currentAccountIndex = i;
                printf("Login successful.\n");
                return 1;
            }
            else {
                printf("Incorrect password.\n");
                return 0;
            }
        }
    }

    printf("Account not found.\n");
    return 0;
}

void logout(void) {
    currentAccountIndex = -1;
}

int getCurrentSessionAccountNumber(void) {
    if (currentAccountIndex == -1) return -1;
    return accounts[currentAccountIndex].accountNumber;
}

int deleteCurrentAccount(void) {
    if (currentAccountIndex == -1) {
        printf("No account logged in.\n");
        return 0;
    }

    char password[50];
    readLine("Enter password to confirm deletion: ", password, sizeof(password));

    if (strcmp(accounts[currentAccountIndex].password, password) != 0) {
        printf("Incorrect password. Deletion cancelled.\n");
        return 0;
    }

    char reason[100];
    readLine("Reason for deletion: ", reason, sizeof(reason));

    accounts[currentAccountIndex].isActive = 0;
    currentAccountIndex = -1;

    printf("Account deleted successfully.\n");
    return 1;
}

Account* findAccountByNumber(int accountNumber) {
    for (int i = 0; i < accountCount; i++) {
        if (accounts[i].accountNumber == accountNumber && accounts[i].isActive) {
            return &accounts[i];
        }
    }
    return NULL;
}

static int isStrongPassword(const char* password) {
    int hasLetter = 0, hasDigit = 0;

    if (strlen(password) < 6) return 0;

    for (int i = 0; password[i]; i++) {
        if (isalpha(password[i])) hasLetter = 1;
        if (isdigit(password[i])) hasDigit = 1;
    }

    return hasLetter && hasDigit;
}

static int generateAccountNumber(void) {
    static int next = 1000;
    while (accountNumberExists(next)) next++;
    return next++;
}

static int accountNumberExists(int accountNumber) {
    for (int i = 0; i < accountCount; i++) {
        if (accounts[i].accountNumber == accountNumber) return 1;
    }
    return 0;
}

static void readLine(const char* prompt, char* buffer, size_t size) {
    printf("%s", prompt);
    fgets(buffer, size, stdin);
    buffer[strcspn(buffer, "\n")] = 0;
}