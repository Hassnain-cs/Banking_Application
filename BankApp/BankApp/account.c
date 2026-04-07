#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include <conio.h> // for masked input

#include "account.h"

int accountCount = 0;
Account accounts[MAX_ACCOUNTS];

static int currentAccountIndex = -1;

static int generateAccountNumber(void);
static void readLine(const char* prompt, char* buffer, size_t size);
static void readPassword(const char* prompt, char* buffer, int maxLen);
static int accountNumberExists(int accountNumber);
static int isStrongPassword(const char* password);
static int isValidDate(int d, int m, int y);
static int calculateAge(int d, int m, int y);
static void editMenu(Account* acc);

int addAccount(const Account* account) {
    if (accountCount >= MAX_ACCOUNTS) return 0;
    accounts[accountCount++] = *account;
    return 1;
}

/*
Reads password input and masks characters with '*'
Supports backspace handling
*/
static void readPassword(const char* prompt, char* buffer, int maxLen) {

    printf("%s", prompt);

    int i = 0;
    char ch;

    while (1) {
        ch = _getch(); // read character without echo

        if (ch == '\r') { // Enter key
            buffer[i] = '\0';
            printf("\n");
            break;
        }
        else if (ch == '\b') { // Backspace
            if (i > 0) {
                i--;
                printf("\b \b"); // erase character
            }
        }
        else if (i < maxLen - 1) {
            buffer[i++] = ch;
            printf("*"); // show mask
        }
    }
}

/*
Account creation with masked password input
*/
void createAccount(void) {

    if (accountCount >= MAX_ACCOUNTS) {
        printf("Maximum number of accounts reached.\n");
        return;
    }

    Account newAccount;
    memset(&newAccount, 0, sizeof(newAccount));

    newAccount.accountNumber = generateAccountNumber();
    newAccount.balance = 100.0;
    newAccount.isActive = 1;

    readLine("Enter First Name: ", newAccount.firstName, sizeof(newAccount.firstName));
    readLine("Enter Last Name: ", newAccount.lastName, sizeof(newAccount.lastName));

    while (1) {
        printf("Enter DOB (DD MM YYYY - use spaces): ");

        if (scanf_s("%d %d %d",
            &newAccount.birthDay,
            &newAccount.birthMonth,
            &newAccount.birthYear) != 3) {

            printf("Invalid format. Please enter as: DD MM YYYY\n");
            while (getchar() != '\n');
            continue;
        }
        while (getchar() != '\n');

        if (!isValidDate(newAccount.birthDay, newAccount.birthMonth, newAccount.birthYear)) {
            printf("Invalid date.\n");
            continue;
        }

        if (calculateAge(newAccount.birthDay, newAccount.birthMonth, newAccount.birthYear) < 18) {
            printf("You must be at least 18 years old.\n");
            continue;
        }

        break;
    }

    readLine("Enter Address: ", newAccount.address, sizeof(newAccount.address));

    while (1) {

        printf("\n----- CONFIRM DETAILS -----\n");
        printf("First Name: %s\n", newAccount.firstName);
        printf("Last Name : %s\n", newAccount.lastName);
        printf("DOB       : %02d/%02d/%04d\n",
            newAccount.birthDay,
            newAccount.birthMonth,
            newAccount.birthYear);
        printf("Address   : %s\n", newAccount.address);

        char confirm;
        printf("Confirm details? (Y/N): ");
        scanf_s(" %c", &confirm, 1);
        while (getchar() != '\n');

        if (confirm == 'Y' || confirm == 'y') break;

        editMenu(&newAccount);
    }

    // =========================
    // MASKED PASSWORD INPUT
    // =========================
    char password[50], confirmPassword[50];

    while (1) {
        readPassword("Enter password: ", password, sizeof(password));

        if (!isStrongPassword(password)) {
            printf("Password must be at least 6 characters and contain letters and numbers.\n");
            continue;
        }

        while (1) {
            readPassword("Confirm password: ", confirmPassword, sizeof(confirmPassword));

            if (strcmp(password, confirmPassword) != 0) {
                printf("Passwords do not match. Try confirming again.\n");
                continue;
            }
            break;
        }

        break;
    }

    strcpy(newAccount.password, password);

    accounts[accountCount++] = newAccount;

    printf("\nAccount created successfully.\n");
    printf("Account Number: %d\n", newAccount.accountNumber);
    printf("Starting Balance: $%.2f\n", newAccount.balance);
}

/*
Login now uses masked password input
*/
int login(void) {

    char line[100];
    int accNum;
    char password[50];

    printf("Enter account number: ");
    if (!fgets(line, sizeof(line), stdin)) return 0;

    if (sscanf(line, "%d", &accNum) != 1) {
        printf("Invalid account number.\n");
        return 0;
    }

    readPassword("Enter password: ", password, sizeof(password));

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

/*
Delete account confirmation also masked
*/
int deleteCurrentAccount(void) {

    if (currentAccountIndex == -1) {
        printf("No account logged in.\n");
        return 0;
    }

    char password[50];
    readPassword("Enter password to confirm deletion: ", password, sizeof(password));

    if (strcmp(accounts[currentAccountIndex].password, password) != 0) {
        printf("Incorrect password. Deletion cancelled.\n");
        return 0;
    }

    accounts[currentAccountIndex].isActive = 0;
    currentAccountIndex = -1;

    printf("Account deleted successfully.\n");
    return 1;
}

void logout(void) {
    currentAccountIndex = -1;
}

int getCurrentSessionAccountNumber(void) {
    if (currentAccountIndex == -1) return -1;
    return accounts[currentAccountIndex].accountNumber;
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

static int isValidDate(int d, int m, int y) {
    if (y < 1900 || m < 1 || m > 12 || d < 1 || d > 31) return 0;
    return 1;
}

static int calculateAge(int d, int m, int y) {
    time_t t = time(NULL);
    struct tm* now = localtime(&t);

    int age = now->tm_year + 1900 - y;

    if ((now->tm_mon + 1 < m) ||
        (now->tm_mon + 1 == m && now->tm_mday < d)) {
        age--;
    }

    return age;
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
    if (fgets(buffer, size, stdin)) {
        buffer[strcspn(buffer, "\n")] = 0;
    }
}

static void editMenu(Account* acc) {

    int choice;

    printf("\n--- EDIT MENU ---\n");
    printf("1) First Name\n");
    printf("2) Last Name\n");
    printf("3) Date of Birth\n");
    printf("4) Address\n");
    printf("5) Back\n");

    printf("Choose option: ");
    scanf_s("%d", &choice);
    while (getchar() != '\n');

    switch (choice) {

    case 1:
        printf("Current First Name: %s\n", acc->firstName);
        readLine("Enter new First Name: ", acc->firstName, sizeof(acc->firstName));
        break;

    case 2:
        printf("Current Last Name: %s\n", acc->lastName);
        readLine("Enter new Last Name: ", acc->lastName, sizeof(acc->lastName));
        break;

    case 3:
        printf("Enter DOB (DD MM YYYY - use spaces): ");
        scanf_s("%d %d %d",
            &acc->birthDay,
            &acc->birthMonth,
            &acc->birthYear);
        while (getchar() != '\n');
        break;

    case 4:
        printf("Current Address: %s\n", acc->address);
        readLine("Enter new Address: ", acc->address, sizeof(acc->address));
        break;

    case 5:
        return;
    }
}