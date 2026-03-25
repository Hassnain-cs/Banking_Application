#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>

#include "account.h"
#include "model.h"

int accountCount = 0;
Account accounts[MAX_ACCOUNTS];

static int currentAccountIndex = -1;

static int generateAccountNumber(void);
static void readLine(const char* prompt, char* buffer, size_t size);
static int accountNumberExists(int accountNumber);

void createAccount(void) {
    if (accountCount >= MAX_ACCOUNTS) {
        printf("Maximum number of accounts reached.\n");
        return;
    }

    Account newAccount;
    newAccount.accountNumber = generateAccountNumber();
    newAccount.balance = 0.0;

    do {
        readLine("Enter account holder's name: ", newAccount.name, sizeof(newAccount.name));
        if (strlen(newAccount.name) == 0) {
            printf("Error: Name cannot be empty. Please try again.\n");
        }
    } while (strlen(newAccount.name) == 0);

    do {
        readLine("Enter password: ", newAccount.password, sizeof(newAccount.password));
        if (strlen(newAccount.password) == 0) {
            printf("Error: Password cannot be empty. Please try again.\n");
        }
    } while (strlen(newAccount.password) == 0);

    newAccount.isActive = 1;

    accounts[accountCount++] = newAccount;

    printf("Account created successfully!\n");
    printf("Account Number: %d\n", newAccount.accountNumber);
    printf("Starting Balance: $%.2f\n", newAccount.balance);
}

int addAccount(const Account* account) {
    if (accountCount >= MAX_ACCOUNTS) {
        return 0;
    }

    accounts[accountCount++] = *account;
    return 1;
}

int login(void) {
    char line[100];
    int accNum;
    char password[50];

    printf("Enter account number: ");
    if (!fgets(line, sizeof(line), stdin)) return 0;

    if (sscanf(line, "%d", &accNum) != 1) {
        printf("Invalid input. Please enter a number.\n");
        return 0;
    }

    do {
        readLine("Enter password: ", password, sizeof(password));
        if (strlen(password) == 0) {
            printf("Error: Password cannot be empty. Please try again.\n");
        }
    } while (strlen(password) == 0);

    int found = 0;

    for (int i = 0; i < accountCount; i++) {
        if (accounts[i].accountNumber == accNum) {
            found = 1;

            if (!accounts[i].isActive) {
                printf("This account is deactivated.\n");
                return 0;
            }

            if (strcmp(accounts[i].password, password) == 0) {
                currentAccountIndex = i;
                printf("Login successful!\n");
                return 1;
            }
            else {
                printf("Incorrect password.\n");
                return 0;
            }
        }
    }

    if (!found) {
        printf("Account not found.\n");
    }

    return 0;
}

void logout(void) {
    currentAccountIndex = -1;
    printf("Logged out.\n");
}

int getCurrentSessionAccountNumber(void) {
    if (currentAccountIndex == -1) {
        return -1;
    }

    return accounts[currentAccountIndex].accountNumber;
}

int deleteCurrentAccount(void) {
    if (currentAccountIndex == -1) {
        printf("No account is currently logged in.\n");
        return 0;
    }

    for (int i = currentAccountIndex; i < accountCount - 1; i++) {
        accounts[i] = accounts[i + 1];
    }

    accountCount--;
    currentAccountIndex = -1;

    printf("Account deleted successfully.\n");
    return 1;
}

Account* findAccountByNumber(int accountNumber) {
    for (int i = 0; i < accountCount; i++) {
        if (accounts[i].accountNumber == accountNumber) {
            return &accounts[i];
        }
    }
    return NULL;
}

static int generateAccountNumber(void) {
    static int nextAccountNumber = 1000;
    int candidate = nextAccountNumber;

    while (accountNumberExists(candidate)) {
        candidate++;
    }

    nextAccountNumber = candidate + 1;
    return candidate;
}

static int accountNumberExists(int accountNumber) {
    for (int i = 0; i < accountCount; i++) {
        if (accounts[i].accountNumber == accountNumber) {
            return 1;
        }
    }
    return 0;
}

static void readLine(const char* prompt, char* buffer, size_t size) {
    printf("%s", prompt);

    if (fgets(buffer, size, stdin)) {
        size_t len = strlen(buffer);
        if (len > 0 && buffer[len - 1] == '\n') {
            buffer[len - 1] = '\0';
        }
    }
}