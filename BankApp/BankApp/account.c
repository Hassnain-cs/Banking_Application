/*
 * @file account.c
 * @brief Implements all account management functions
 * This file contains the implementation of account creation, login,
 * editing, deletion, and all supporting validation functions.
 */


#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include <conio.h> // for _getch()

#include "account.h"


 /* ===============================================================
  * GLOBAL VARIABLES
  * =============================================================== */
  
int accountCount = 0;                  // Tracks the number of accounts currently stored
Account accounts[MAX_ACCOUNTS];       // Array to hold all accounts in memory

static int currentAccountIndex = -1; // Index of currently logged-in account in the accounts array


/* ===============================================================
 * STATIC FUNCTION DECLARATIONS (Private to this file)
 * =============================================================== */

// Function declarations
static int generateAccountNumber(void); // Auto-incrementing account number generator

static void readLine(const char* prompt, char* buffer, size_t size); // Reads a line of input safely

static void readPassword(const char* prompt, char* buffer, int maxLen); // Reads password input with masking

static int accountNumberExists(int accountNumber); // Checks if an account number already exists (to prevent duplicates)

static int isStrongPassword(const char* password); // Validates password strength (min 6 chars, letters + numbers)

// Proper date validation with month rules + leap year (THIS IS REAL VALIDATION)

static int isValidDate(int d, int m, int y) {

	if (y < 1900 || m < 1 || m > 12 || d < 1) // basic checks
        return 0;

	int daysInMonth; // will be set based on month

    switch (m) {
	case 1: case 3: case 5: case 7: case 8: case 10: case 12: // Months with 31 days
        daysInMonth = 31;
        break;

	case 4: case 6: case 9: case 11: // Months with 30 days
        daysInMonth = 30;
        break;

    case 2:
        // Leap year logic

		if ((y % 4 == 0 && y % 100 != 0) || (y % 400 == 0)) // leap year
			daysInMonth = 29; // February in leap year
        else
			daysInMonth = 28; // February in non-leap year
        break;

    default:
        return 0;
    }

	return d <= daysInMonth; // final check based on month
}

// Simple but effective email validation

static int isValidEmail(const char* email) { // checks for presence of '@' and '.' after '@'
	const char* at = strchr(email, '@'); // look for '@'
    if (!at) return 0;

	const char* dot = strchr(at, '.'); // look for '.' after '@'
    if (!dot) return 0;

    if (at == email) return 0; // nothing before @

    return 1;
}

// Phone must be exactly 10 digits
static int isValidPhone(const char* phone) { // must be exactly 10 digits, no other characters
    if (strlen(phone) != 10) return 0;

	for (int i = 0; i < 10; i++) {  // check each character is a digit
        if (!isdigit(phone[i])) return 0;
    }
    return 1;
}

// Street number must be numeric
static int isValidStreetNumber(const char* num) { // must be numeric only
    for (int i = 0; num[i]; i++) {
        if (!isdigit(num[i])) return 0;
    }
    return 1;
}

// Canadian postal code: A1A1A1 format
static int isValidPostalCode(const char* code) { // must be 6 characters, alternating letter-digit
    if (strlen(code) != 6) return 0;

    for (int i = 0; i < 6; i++) {
		if (i % 2 == 0 && !isalpha(code[i])) return 0;  // even index: must be letter
		if (i % 2 == 1 && !isdigit(code[i])) return 0;  // odd index: must be digit
    }
    return 1;
} 
static int calculateAge(int d, int m, int y); // calculates age based on DOB and current date
static void editMenu(Account* acc);           // menu for editing account details during creation confirmation
 

// Add account to array
int addAccount(const Account* account) {  
	if (accountCount >= MAX_ACCOUNTS) return 0; // no space for more accounts
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

	if (accountCount >= MAX_ACCOUNTS) { // prevent overflow
        printf("Maximum number of accounts reached.\n"); 
        return;
    }

    Account newAccount; 
	memset(&newAccount, 0, sizeof(newAccount)); // initialize all fields to zero/empty

	newAccount.accountNumber = generateAccountNumber(); // auto-generate unique account number
	newAccount.balance = 100.0;  // initial balance
	newAccount.isActive = 1;     // mark account as active

	readLine("Enter First Name: ", newAccount.firstName, sizeof(newAccount.firstName));  // readLine is a safer alternative to scanf for strings
	readLine("Enter Last Name: ", newAccount.lastName, sizeof(newAccount.lastName));     // readLine is a safer alternative to scanf for strings
    while (1) {
        printf("Enter DOB (DD MM YYYY): ");

        //  Using scanf (simpler than scanf_s)
        if (scanf("%d %d %d",
            &newAccount.birthDay,
            &newAccount.birthMonth,
            &newAccount.birthYear) != 3) {

			printf("Invalid format.\n"); // if input doesn't match expected format, show error and retry
            while (getchar() != '\n');
            continue;
        }
        while (getchar() != '\n'); // clear buffer

		if (!isValidDate(newAccount.birthDay, newAccount.birthMonth, newAccount.birthYear)) { // validate date with proper rules
            printf("Invalid date.\n");
            continue; 
        }

		if (calculateAge(newAccount.birthDay, newAccount.birthMonth, newAccount.birthYear) < 18) { // check age requirement
            printf("Must be at least 18 years old.\n");
            continue;
        }

        break;
    }

    // EMAIL VALIDATION LOOP
    while (1) {
		readLine("Enter Email Address: ", newAccount.email, sizeof(newAccount.email)); // readLine is a safer alternative to scanf for strings
		if (!isValidEmail(newAccount.email)) { // validate email format
            printf("Invalid email format.\n");
            continue;
        }
        break;
    }

    // PHONE VALIDATION LOOP
    while (1) {
		readLine("Enter Phone Number (10 digits): ", newAccount.phone, sizeof(newAccount.phone)); // readLine is a safer alternative to scanf for strings
		if (!isValidPhone(newAccount.phone)) { // validate phone number format
            printf("Phone must be exactly 10 digits.\n");
            continue;
        }
        break;
    }

    printf("\n--- Address Details ---\n");

    // STREET NUMBER VALIDATION
    while (1) {
		readLine("Street Number: ", newAccount.streetNumber, sizeof(newAccount.streetNumber)); // readLine is a safer alternative to scanf for strings
		if (!isValidStreetNumber(newAccount.streetNumber)) { // validate street number is numeric
            printf("Street number must be numeric.\n");
            continue;
        }
        break;
    }

	readLine("Street Name  : ", newAccount.streetName, sizeof(newAccount.streetName)); // readLine is a safer alternative to scanf for strings
    readLine("City         : ", newAccount.city, sizeof(newAccount.city));

    // NEW FIELD (province)
    readLine("Province     : ", newAccount.country, sizeof(newAccount.country));

    // POSTAL CODE VALIDATION
    while (1) {
        readLine("Postal Code (A1A1A1): ", newAccount.postalCode, sizeof(newAccount.postalCode));
		if (!isValidPostalCode(newAccount.postalCode)) { //  validate postal code format
            printf("Invalid postal code format.\n");
            continue;
        }
        break;
    }
    readLine("Country      : ", newAccount.country, sizeof(newAccount.country));
   

	while (1) { // confirmation loop - shows entered details and allows user to confirm or edit before finalizing account creation

        printf("\n----- CONFIRM DETAILS -----\n");
        printf("First Name: %s\n", newAccount.firstName);
        printf("Last Name : %s\n", newAccount.lastName);
        printf("DOB       : %02d/%02d/%04d\n",
            newAccount.birthDay,
            newAccount.birthMonth,
            newAccount.birthYear);
        printf("Email     : %s\n", newAccount.email);
        printf("Phone     : %s\n", newAccount.phone);

		// FIXED ADDRESS DISPLAY (added province and rearranged)
        printf("Address   : %s %s, %s, %s, %s\n",
            newAccount.streetNumber,
            newAccount.streetName,
            newAccount.city,
            newAccount.country,
            newAccount.postalCode);

		char confirm; // using char for Y/N confirmation
        printf("Confirm details? (Y/N): ");
        scanf(" %c", &confirm); //  using scanf
        while (getchar() != '\n');

        if (confirm == 'Y' || confirm == 'y') break;

        editMenu(&newAccount);
    }

    // Password setup
    char password[50], confirmPassword[50];

    while (1) {
		readPassword("Enter password: ", password, sizeof(password)); // readPassword handles masking and backspace

		if (!isStrongPassword(password)) { // validate password strength (min 6 chars, letters + numbers)
            printf("Password must be at least 6 chars with letters & numbers.\n");
            continue;
        }

		while (1) { // confirmation loop for password - ensures user enters the same password twice to avoid typos
            readPassword("Confirm password: ", confirmPassword, sizeof(confirmPassword));

            if (strcmp(password, confirmPassword) != 0) {
                printf("Passwords do not match.\n");
                continue;
            }
            break;
        }

        break;
    }

	strcpy(newAccount.password, password); // store password in account struct (in a real application, this should be hashed)

    accounts[accountCount++] = newAccount;
    saveAccountsToFile(); // Immediately save after creating account

    printf("\nAccount created successfully.\n");
    printf("Account Number: %d\n", newAccount.accountNumber);
}


// LOGIN FUNCTION - prompts user for account number and password, validates credentials, and sets current session if successful
int login(void) {

	char line[100]; // buffer for reading input
	int accNum;     // variable to hold parsed account number
	char password[50];  // buffer for password input

    printf("Enter account number (0 to cancel): ");
	if (!fgets(line, sizeof(line), stdin)) return 0; // read input as string to validate format

	if (sscanf(line, "%d", &accNum) != 1) { // validate that input is a number
        printf("Invalid input.\n");
        return 0;
    }

    if (accNum == 0) {
		printf("Login cancelled.\n"); // allow user to cancel login by entering 0
        return 0;
    }

	readPassword("Enter password: ", password, sizeof(password)); // readPassword handles masking and backspace

	for (int i = 0; i < accountCount; i++) { // search for account with matching account number that is active
        if (accounts[i].accountNumber == accNum && accounts[i].isActive) {
			if (strcmp(accounts[i].password, password) == 0) { // if password matches, set current session to this account
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

void editCurrentAccount(void) { // allows logged-in user to edit their account details with validation and confirmation

	if (currentAccountIndex == -1) { // ensure user is logged in before allowing edits
        printf("No account logged in.\n");
        return;
    }

	Account* acc = &accounts[currentAccountIndex]; // pointer to current account for easier access

    int choice;
    int updated = 0;

	while (1) { // edit loop - shows current details and menu until user chooses to go back, tracks if any updates were made to save at the end

        printf("\n========= CURRENT DETAILS =========\n");
        printf("First Name : %s\n", acc->firstName);
        printf("Last Name  : %s\n", acc->lastName);
        printf("DOB        : %02d/%02d/%04d\n", acc->birthDay, acc->birthMonth, acc->birthYear);
        printf("Email      : %s\n", acc->email);
        printf("Phone      : %s\n", acc->phone);
        printf("Address    : %s %s, %s, %s, %s\n",
            acc->streetNumber,
            acc->streetName,
            acc->city,
            acc->country,
            acc->postalCode);
        printf("===================================\n");

        printf("\n--- EDIT MENU ---\n");
        printf("1) First Name\n");
        printf("2) Last Name\n");
        printf("3) DOB\n");
        printf("4) Email\n");
        printf("5) Phone\n");
        printf("6) Address\n");
        printf("7) Password\n");
        printf("0) Back\n");

        printf("Choose option: ");


		// FIXED VALIDATION (matches menu options)

		if (scanf_s("%d", &choice) != 1) {  // validate that input is a number
            while (getchar() != '\n');
            continue;
        }
		while (getchar() != '\n'); // clear buffer

        if (choice == 0) {
            if (updated) {
                saveAccountsToFile();
                printf("Account updated successfully.\n");
            }
            else {
                printf("No changes made.\n");
            }
            return;
        }

        switch (choice) {
             
		case 1: // edit first name
            readLine("New First Name: ", acc->firstName, sizeof(acc->firstName));
            updated = 1;
            break;

		case 2: //  edit last name
            readLine("New Last Name: ", acc->lastName, sizeof(acc->lastName));
            updated = 1;
            break;

		case 3: // edit DOB with full validation
            while (1) {
                printf("Enter DOB (DD MM YYYY) (0 to cancel): ");
                if (scanf("%d %d %d", &acc->birthDay, &acc->birthMonth, &acc->birthYear) != 3) {
                    while (getchar() != '\n');
                    continue;
                }
                while (getchar() != '\n');

                if (acc->birthDay == 0) break;

                if (!isValidDate(acc->birthDay, acc->birthMonth, acc->birthYear)) {
                    printf("Invalid date.\n");
                    continue;
                }
                break;
            }
            updated = 1;
            break;

		case 4: // edit email with validation
            while (1) {
                readLine("New Email (0 to cancel): ", acc->email, sizeof(acc->email));
                if (strcmp(acc->email, "0") == 0) break;
                if (!isValidEmail(acc->email)) {
                    printf("Invalid email.\n");
                    continue;
                }
                updated = 1;
                break;
            }
            break;

		case 5: // edit phone with validation
            while (1) {
                readLine("New Phone (10 digits, 0 to cancel): ", acc->phone, sizeof(acc->phone));
                if (strcmp(acc->phone, "0") == 0) break;
                if (!isValidPhone(acc->phone)) {
                    printf("Invalid phone.\n");
                    continue;
                }
                updated = 1;
                break;
            }
            break;

		case 6: // edit address with validation for street number and postal code
            readLine("Street Number (0 to cancel): ", acc->streetNumber, sizeof(acc->streetNumber));
            if (strcmp(acc->streetNumber, "0") == 0) break;

            readLine("Street Name: ", acc->streetName, sizeof(acc->streetName));
            readLine("City: ", acc->city, sizeof(acc->city));
            readLine("Province: ", acc->country, sizeof(acc->country));
            readLine("Postal Code: ", acc->postalCode, sizeof(acc->postalCode));

            updated = 1;
            break;

		case 7: // edit password with strength validation and confirmation
            readPassword("New Password: ", acc->password, sizeof(acc->password));
            updated = 1;
            break;

        default:
            printf("Invalid option.\n");
        }
    }
}


// Delete Account - marks account as inactive after confirming password and user intent, then logs out
int deleteCurrentAccount(void) {

	if (currentAccountIndex == -1) { // ensure user is logged in before allowing deletion
        printf("No account logged in.\n");
        return 0;
    }

	char confirm; // using char for Y/N confirmation
    printf("Are you sure you want to delete? (Y/N): ");
    scanf_s(" %c", &confirm, 1);
    while (getchar() != '\n');

    if (confirm == 'N' || confirm == 'n') {
        printf("Deletion cancelled.\n");
        return 0;
    }

	char password[50]; // buffer for password input
    readPassword("Confirm password: ", password, sizeof(password));

    if (strcmp(accounts[currentAccountIndex].password, password) != 0) {
        printf("Incorrect password.\n");
        return 0;
    }

	accounts[currentAccountIndex].isActive = 0; // mark account as inactive (soft delete)
    currentAccountIndex = -1;

    saveAccountsToFile();

    printf("Account deleted.\n");
    return 1;
}


// Other helper functions (unchanged mostly)

void logout(void) {
    currentAccountIndex = -1;
}

int getCurrentSessionAccountNumber(void) { // returns account number of currently logged-in user, or -1 if no user is logged in
    if (currentAccountIndex == -1) return -1;
    return accounts[currentAccountIndex].accountNumber;
}

Account* findAccountByNumber(int accountNumber) { // searches for an active account with the given account number and returns a pointer to it, or NULL if not found
    for (int i = 0; i < accountCount; i++) {
        if (accounts[i].accountNumber == accountNumber && accounts[i].isActive) {
            return &accounts[i];
        }
    }
    return NULL;
}

static int isStrongPassword(const char* password) { // checks if password is at least 6 characters and contains both letters and digits
    int hasLetter = 0, hasDigit = 0;

    if (strlen(password) < 6) return 0;

	for (int i = 0; password[i]; i++) { // check each character for letter and digit
        if (isalpha(password[i])) hasLetter = 1;
        if (isdigit(password[i])) hasDigit = 1;
    }

    return hasLetter && hasDigit;
}

static int calculateAge(int d, int m, int y) { // calculates age based on DOB and current date, accounting for month/day to determine if birthday has occurred this year
    time_t t = time(NULL);
    struct tm* now = localtime(&t);

    int age = now->tm_year + 1900 - y;
     
	if ((now->tm_mon + 1 < m) || // if current month is before birth month, or if it's the birth month but current day is before birth day, then birthday hasn't occurred yet this year, so subtract 1 from age
        (now->tm_mon + 1 == m && now->tm_mday < d)) {
        age--;
    }

    return age;
}

static int generateAccountNumber(void) { // generates a unique account number starting from 1000, incrementing until it finds an unused number
    static int next = 1000;
    while (accountNumberExists(next)) next++;
    return next++;
}

static int accountNumberExists(int accountNumber) { // checks if an account number already exists in the accounts array, used to ensure uniqueness when generating new account numbers
    for (int i = 0; i < accountCount; i++) {
        if (accounts[i].accountNumber == accountNumber) return 1;
    }
    return 0;
}

static void readLine(const char* prompt, char* buffer, size_t size) {
	printf("%s", prompt); // display prompt to user
	if (fgets(buffer, size, stdin)) { // read input as string, fgets is safer than scanf for strings
        buffer[strcspn(buffer, "\n")] = 0;
    }
}


//EDIT MENU - allows user to edit specific fields of their account with validation, called during account creation confirmation and from the main edit menu
static void editMenu(Account* acc) {

	int choice; /// variable to hold user's menu choice

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

	case 1: // edit first name
        readLine("New First Name: ", acc->firstName, sizeof(acc->firstName));
        break;

	case 2: // edit last name
        readLine("New Last Name: ", acc->lastName, sizeof(acc->lastName));
        break;

	case 3: // edit DOB with validation
        printf("Enter DOB (DD MM YYYY): ");
        scanf("%d %d %d", &acc->birthDay, &acc->birthMonth, &acc->birthYear);
        while (getchar() != '\n');
        break;

	case 4: // edit email with validation
        while (1) {
            readLine("New Email: ", acc->email, sizeof(acc->email));
            if (!isValidEmail(acc->email)) {
                printf("Invalid email.\n");
                continue;
            }
            break;
        }
        break;

	case 5: // edit phone with validation
        while (1) {
            readLine("New Phone (10 digits): ", acc->phone, sizeof(acc->phone));
            if (!isValidPhone(acc->phone)) {
                printf("Invalid phone.\n");
                continue;
            }
            break;
        }
        break;

	case 6: { // edit address with validation for street number and postal code
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
		case 1: // edit street number with validation
            readLine("Street Number: ", acc->streetNumber, sizeof(acc->streetNumber));
            break;
		case 2: // edit street name
            readLine("Street Name: ", acc->streetName, sizeof(acc->streetName));
            break;
		case 3: // edit city
            readLine("City: ", acc->city, sizeof(acc->city));
            break;
		case 4: // edit province
            readLine("Province: ", acc->country, sizeof(acc->country));
            break;
		case 5: // edit postal code with validation
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