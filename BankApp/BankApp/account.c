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
	int max = 10000;
	int i;
	for (i = 0; i < g_accountCount; i++) {
		if (g_accounts[i].accountNumber > max) {
			max = g_accounts[i].accountNumber;
		}
	}
	if (max >= INT_MAX) {
		return max + 1;
	}
}

void createAccount(void) {
	Account account;
	memset(&account, 0, sizeof(Account));

	printf("\n--- Create Account ---\n");

	int newNumber = nextAccountNumber();
	if (newNumber == -1) {
		printf("Error: Maximum number of accounts reached.\n");
		return;
	}
	account.accountNumber = newNumber;

	readLine("Enter name: ", account.name, (int)sizeof(account.name));
	readLine("Password: ", account.password, (int)sizeof(account.password));

	account.balance = 0.0;
	account.isActive = 1;

	if (!addAccount(&account)) {
		printf("Error: Could not create account. Please try again.\n");
		return;
	}

	printf("\nAccount created successfully! Your account number is: %d\n", account.accountNumber);

}
	int login(void) {
		const int MAX_LOGIN_ATTEMPTS = 3;

		if (g_accountCount <= 0) {
			printf("\nNo accounts available. Please create an account first.\n");
			return -1;
		}

		int attempt;
		for (attempt = 1; attempt <= MAX_LOGIN_ATTEMPTS; attempt++) {
			printf("\n--- Login (Attempt %d of %d) ---\n", attempt, MAX_LOGIN_ATTEMPTS);
			int accNum = getIntInRange("Enter account number: ", 1, INT_MAX);

			char password[MAX_PASSWORD];
			readLine("Enter password: ", password, (int)sizeof(password));

			Account* account = findAccountByNumber(accNum);
			if (!account) {
				printf("Invalid account number. Please try again.\n");
				continue;
			}

			if (!account->isActive) {
				printf("This account is inactive. Please contact support.\n");
				continue;
			}

			if(strcmp(account->password, password) != 0) {
		
				printf("Incorrect password. Please try again.\n");
			}

			g_currentSessionAccountNumber = account->accountNumber;
			return account->accountNumber;
		}

		printf("\nMaximum login attempts reached. Please try again later.\n");
		return -1;
	}