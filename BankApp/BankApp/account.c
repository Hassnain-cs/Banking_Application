#include <stdio.h>
#include <string.h>
#include <limits.h>

#include "account.h"
#include "validation.h"

Account g_accounts[MAX_ACCOUNTS];
int g_accountCount = 0;

static int g_currentSessionAccountNumber = -1;

int getCurrentSessionAccountNumber(void) {
	return g_currentSessionAccountNumber;
}

void logout(void) {
	g_currentSessionAccountNumber = -1;
}

Account* findAccountByNumber(int accountNumber) {
	int i;
	for (i = 0; i < g_accountCount; i++) {
		if (g_accounts[i].accountNumber == accountNumber) {
			return &g_accounts[i];
		}
	}
	return NULL;
}

int addAccount(const Account* account) {
	if (!account)
		return 0;
	if (g_accountCount >= MAX_ACCOUNTS)
		return 0;
	if (findAccountByNumber(account->accountNumber)!=NULL)
		return 0; 

	g_accounts[g_accountCount++] = *account;
	g_accountCount++;
	return 1;
}

static int nextAccountNumber(void) {
	int max = 100000;              // start range (so first becomes 100001)
	for (int i = 0; i < g_accountCount; i++) {
		if (g_accounts[i].accountNumber > max) max = g_accounts[i].accountNumber;
	}
	return max + 1;
}

static int g_rngSeeded = 0;

static void seedRngOnce(void) {
	if (!g_rngSeeded) {
		srand((unsigned)time(NULL));
		g_rngSeeded = 1;
	}
}

static int generateRandomAccountNumber(void) {
	seedRngOnce();

	
	for (int tries = 0; tries < 1000; tries++) {
		int candidate = 100000 + (rand() % 900000);
		if (findAccountByNumber(candidate) == NULL) {
			return candidate;
		}
	}
	return -1; 
}

void createAccount(void) {
	Account account = (Account){ 0 };

	printf("\n--- Create Account ---\n");
	readLine("Enter name: ", account.name, (int)sizeof(account.name));
	readLine("Password: ", account.password, (int)sizeof(account.password));

	int accNum = generateRandomAccountNumber();
	if (accNum == -1) {
		printf("Failed to generate unique account number.\n");
		return;
	}

	account.accountNumber = accNum;
	account.balance = 0.0;
	account.isActive = 1;

	if (!addAccount(&account)) {
		printf("Failed to create account.\n");
		return;
	}

	printf("\nAccount created successfully! Your account number is: %d\n", account.accountNumber);
}

int login(void) {
	const int MAX_TRIES = 3;

	for (int attempt = 1; attempt <= MAX_TRIES; attempt++) {
		printf("\n--- Login (Attempt %d of %d) ---\n", attempt, MAX_TRIES);

		int accountNumber = getIntInRange("Enter account number: ", 1, INT_MAX);

		char entered[MAX_PASSWORD];
		readLine("Enter password: ", entered, (int)sizeof(entered));

		Account* acc = findAccountByNumber(accountNumber);
		if (!acc) {
			printf("Account not found. Please try again.\n");
			continue;
		}

		if (strcmp(acc->password, entered) != 0) {
			printf("Incorrect password. Please try again.\n");
			continue;
		}

		
		return acc->accountNumber;
	}

	printf("\nToo many failed attempts.\n");
	return -1;
}