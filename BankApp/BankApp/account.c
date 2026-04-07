#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include <conio.h> // for _getch()

#include "account.h"

int accountCount = 0;
Account accounts[MAX_ACCOUNTS];

static int currentAccountIndex = -1;

// Function declarations
static int generateAccountNumber(void);
static void readLine(const char* prompt, char* buffer, size_t size);
static void readPassword(const char* prompt, char* buffer, int maxLen);
static int accountNumberExists(int accountNumber);
static int isStrongPassword(const char* password);
// Proper date validation with month rules + leap year (THIS IS REAL VALIDATION NOW)
static int isValidDate(int d, int m, int y) {

    if (y < 1900 || m < 1 || m > 12 || d < 1)
        return 0;

    int daysInMonth;

    switch (m) {
    case 1: case 3: case 5: case 7: case 8: case 10: case 12:
        daysInMonth = 31;
        break;

    case 4: case 6: case 9: case 11:
        daysInMonth = 30;
        break;

    case 2:
        // Leap year logic (VERY IMPORTANT)
        if ((y % 4 == 0 && y % 100 != 0) || (y % 400 == 0))
            daysInMonth = 29;
        else
            daysInMonth = 28;
        break;

    default:
        return 0;
    }

    return d <= daysInMonth;
}

// Simple but effective email validation
static int isValidEmail(const char* email) {
    const char* at = strchr(email, '@');
    if (!at) return 0;

    const char* dot = strchr(at, '.');
    if (!dot) return 0;

    if (at == email) return 0; // nothing before @

    return 1;
}

// Phone must be exactly 10 digits
static int isValidPhone(const char* phone) {
    if (strlen(phone) != 10) return 0;

    for (int i = 0; i < 10; i++) {
        if (!isdigit(phone[i])) return 0;
    }
    return 1;
}

// Street number must be numeric
static int isValidStreetNumber(const char* num) {
    for (int i = 0; num[i]; i++) {
        if (!isdigit(num[i])) return 0;
    }
    return 1;
}

// Canadian postal code: A1A1A1 format
static int isValidPostalCode(const char* code) {
    if (strlen(code) != 6) return 0;

    for (int i = 0; i < 6; i++) {
        if (i % 2 == 0 && !isalpha(code[i])) return 0;
        if (i % 2 == 1 && !isdigit(code[i])) return 0;
    }
    return 1;
}
static int calculateAge(int d, int m, int y);
static void editMenu(Account* acc);


// Add account to array
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
                printf("\b \b");
            }
        }
        //  FIX: only allow printable characters
        else if (isprint(ch) && i < maxLen - 1) {
            buffer[i++] = ch;
            printf("*");
        }
    }
}


/*
Account creation
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
        printf("Enter DOB (DD MM YYYY): ");

        //  Using scanf (simpler than scanf_s)
        if (scanf("%d %d %d",
            &newAccount.birthDay,
            &newAccount.birthMonth,
            &newAccount.birthYear) != 3) {

            printf("Invalid format.\n");
            while (getchar() != '\n');
            continue;
        }
        while (getchar() != '\n'); // clear buffer

        if (!isValidDate(newAccount.birthDay, newAccount.birthMonth, newAccount.birthYear)) {
            printf("Invalid date.\n");
            continue;
        }

        if (calculateAge(newAccount.birthDay, newAccount.birthMonth, newAccount.birthYear) < 18) {
            printf("Must be at least 18 years old.\n");
            continue;
        }

        break;
    }

    // EMAIL VALIDATION LOOP
    while (1) {
        readLine("Enter Email Address: ", newAccount.email, sizeof(newAccount.email));
        if (!isValidEmail(newAccount.email)) {
            printf("Invalid email format.\n");
            continue;
        }
        break;
    }

    // PHONE VALIDATION LOOP
    while (1) {
        readLine("Enter Phone Number (10 digits): ", newAccount.phone, sizeof(newAccount.phone));
        if (!isValidPhone(newAccount.phone)) {
            printf("Phone must be exactly 10 digits.\n");
            continue;
        }
        break;
    }

    printf("\n--- Address Details ---\n");

    // STREET NUMBER VALIDATION
    while (1) {
        readLine("Street Number: ", newAccount.streetNumber, sizeof(newAccount.streetNumber));
        if (!isValidStreetNumber(newAccount.streetNumber)) {
            printf("Street number must be numeric.\n");
            continue;
        }
        break;
    }

    readLine("Street Name  : ", newAccount.streetName, sizeof(newAccount.streetName));
    readLine("City         : ", newAccount.city, sizeof(newAccount.city));

    // NEW FIELD (province)
    readLine("Province     : ", newAccount.country, sizeof(newAccount.country));

    // POSTAL CODE VALIDATION
    while (1) {
        readLine("Postal Code (A1A1A1): ", newAccount.postalCode, sizeof(newAccount.postalCode));
        if (!isValidPostalCode(newAccount.postalCode)) {
            printf("Invalid postal code format.\n");
            continue;
        }
        break;
    }
    readLine("Country      : ", newAccount.country, sizeof(newAccount.country));
   

    while (1) {

        printf("\n----- CONFIRM DETAILS -----\n");
        printf("First Name: %s\n", newAccount.firstName);
        printf("Last Name : %s\n", newAccount.lastName);
        printf("DOB       : %02d/%02d/%04d\n",
            newAccount.birthDay,
            newAccount.birthMonth,
            newAccount.birthYear);
        printf("Email     : %s\n", newAccount.email);
        printf("Phone     : %s\n", newAccount.phone);

        //  FIX: Proper address printing (no acc->address)
        printf("Address   : %s %s, %s, %s, %s\n",
            newAccount.streetNumber,
            newAccount.streetName,
            newAccount.city,
            newAccount.country,
            newAccount.postalCode);

        char confirm;
        printf("Confirm details? (Y/N): ");
        scanf(" %c", &confirm); //  using scanf
        while (getchar() != '\n');

        if (confirm == 'Y' || confirm == 'y') break;

        editMenu(&newAccount);
    }

    // Password setup
    char password[50], confirmPassword[50];

    while (1) {
        readPassword("Enter password: ", password, sizeof(password));

        if (!isStrongPassword(password)) {
            printf("Password must be at least 6 chars with letters & numbers.\n");
            continue;
        }

        while (1) {
            readPassword("Confirm password: ", confirmPassword, sizeof(confirmPassword));

            if (strcmp(password, confirmPassword) != 0) {
                printf("Passwords do not match.\n");
                continue;
            }
            break;
        }

        break;
    }

    strcpy(newAccount.password, password);

    accounts[accountCount++] = newAccount;
    saveAccountsToFile(); // Immediately save after creating account

    printf("\nAccount created successfully.\n");
    printf("Account Number: %d\n", newAccount.accountNumber);
}


/*
Login
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

void editCurrentAccount(void) {

    if (currentAccountIndex == -1) {
        printf("No account logged in.\n");
        return;
    }

    Account* acc = &accounts[currentAccountIndex];

    printf("\n===== CURRENT ACCOUNT DETAILS =====\n");
    printf("First Name : %s\n", acc->firstName);
    printf("Last Name  : %s\n", acc->lastName);
    printf("City       : %s\n", acc->city);
    printf("===================================\n");

    int choice;
    int updated = 0; // TRACK IF ANY CHANGE WAS MADE

    while (1) {

        printf("\n--- EDIT MENU ---\n");
        printf("1) Edit First Name\n");
        printf("2) Edit Last Name\n");
        printf("3) Edit City\n");
        printf("4) Edit Password\n");
        printf("7) Back\n");

        printf("Choose option: ");

        if (scanf_s("%d", &choice) != 1) {
            printf("Invalid input.\n");
            while (getchar() != '\n');
            continue;
        }
        while (getchar() != '\n');

        if (choice == 7) {
            if (updated) {
                saveAccountsToFile();
                printf("Account updated successfully.\n");
            }
            else {
                printf("No changes made.\n"); // FIXED FAKE MESSAGE
            }
            return;
        }

        if (choice == 1) {
            printf("Enter new first name: ");
            fgets(acc->firstName, sizeof(acc->firstName), stdin);
            acc->firstName[strcspn(acc->firstName, "\n")] = 0;
            updated = 1;
        }
        else if (choice == 2) {
            printf("Enter new last name: ");
            fgets(acc->lastName, sizeof(acc->lastName), stdin);
            acc->lastName[strcspn(acc->lastName, "\n")] = 0;
            updated = 1;
        }
        else if (choice == 3) {
            printf("Enter new city: ");
            fgets(acc->city, sizeof(acc->city), stdin);
            acc->city[strcspn(acc->city, "\n")] = 0;
            updated = 1;
        }
        else if (choice == 4) {
            printf("Enter new password: ");
            fgets(acc->password, sizeof(acc->password), stdin);
            acc->password[strcspn(acc->password, "\n")] = 0;
            updated = 1;
        }
        else {
            printf("Invalid option.\n");
        }
    }
}


/*
Delete account
*/
int deleteCurrentAccount(void) {

    if (currentAccountIndex == -1) {
        printf("No account logged in.\n");
        return 0;
    }

    char password[50];
    readPassword("Confirm password: ", password, sizeof(password));

    if (strcmp(accounts[currentAccountIndex].password, password) != 0) {
        printf("Incorrect password.\n");
        return 0;
    }

    accounts[currentAccountIndex].isActive = 0;
    currentAccountIndex = -1;

    printf("Account deleted.\n");
    return 1;
}


// Other helper functions (unchanged mostly)

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


/*
EDIT MENU
*/
static void editMenu(Account* acc) {

    int choice;

    printf("\n--- EDIT MENU ---\n");
    printf("1) First Name\n");
    printf("2) Last Name\n");
    printf("3) Date of Birth\n");
    printf("4) Email\n");
    printf("5) Phone\n");
    printf("6) Address\n");
    printf("7) Back\n");

    printf("Choose option: ");
    scanf("%d", &choice);
    while (getchar() != '\n');

    switch (choice) {

    case 1:
        readLine("New First Name: ", acc->firstName, sizeof(acc->firstName));
        break;

    case 2:
        readLine("New Last Name: ", acc->lastName, sizeof(acc->lastName));
        break;

    case 3:
        printf("Enter DOB (DD MM YYYY): ");
        scanf("%d %d %d", &acc->birthDay, &acc->birthMonth, &acc->birthYear);
        while (getchar() != '\n');
        break;

    case 4:
        while (1) {
            readLine("New Email: ", acc->email, sizeof(acc->email));
            if (!isValidEmail(acc->email)) {
                printf("Invalid email.\n");
                continue;
            }
            break;
        }
        break;

    case 5:
        while (1) {
            readLine("New Phone (10 digits): ", acc->phone, sizeof(acc->phone));
            if (!isValidPhone(acc->phone)) {
                printf("Invalid phone.\n");
                continue;
            }
            break;
        }
        break;

    case 6: {
        int sub;
        printf("\n--- ADDRESS MENU ---\n");
        printf("1) Street Number\n");
        printf("2) Street Name\n");
        printf("3) City\n");
        printf("4) Province\n");
        printf("5) Postal Code\n");
        printf("6) Back\n");

        scanf("%d", &sub);
        while (getchar() != '\n');

        switch (sub) {
        case 1:
            readLine("Street Number: ", acc->streetNumber, sizeof(acc->streetNumber));
            break;
        case 2:
            readLine("Street Name: ", acc->streetName, sizeof(acc->streetName));
            break;
        case 3:
            readLine("City: ", acc->city, sizeof(acc->city));
            break;
        case 4:
            readLine("Province: ", acc->country, sizeof(acc->country));
            break;
        case 5:
            readLine("Postal Code: ", acc->postalCode, sizeof(acc->postalCode));
            break;
        }
        break;
    }

    case 7:
        return;
    }
}


/*
FUNCTION: saveAccountsToFile
PURPOSE:
    Saves all accounts into a file so data persists after program closes

HOW IT WORKS:
    - Opens file in write mode
    - Loops through all accounts
    - Writes each account in structured format
    - Closes file
*/
void saveAccountsToFile(void) { //Line 542

    // Open file in write mode (creates file automatically if not exists)
    FILE* f = fopen(ACCOUNTS_FILE, "w");

    // Safety check: if file can't open, don't crash program
    if (!f) {
        printf("Error: Could not save accounts.\n");
        return;
    }

    // Loop through all accounts
    for (int i = 0; i < accountCount; i++) {

        Account* a = &accounts[i];

        // Write account data in ONE LINE per account
        fprintf(f,
            "%d|%s|%s|%d|%d|%d|%s|%s|%s|%s|%s|%s|%.2f|%d\n",
            a->accountNumber,
            a->firstName,
            a->lastName,
            a->birthDay,
            a->birthMonth,
            a->birthYear,
            a->streetNumber,
            a->streetName,
            a->city,
            a->country,
            a->postalCode,
            a->password,
            a->balance,
            a->isActive
        );
    }

    // Always close file after writing
    fclose(f);
}