#include <stdio.h>
#include <string.h>

#include "account.h"
#include "model.h"

int accountCount = 0;
Account accounts[MAX_ACCOUNTS];

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

	readLine("Enter account holder's name: ", newAccount.name, sizeof(newAccount.name));
	readLine("Enter password: ", newAccount.password, sizeof(newAccount.password));
	newAccount.isActive = 1;

	accounts[accountCount++] = newAccount;
	accountCount++;

	printf("Account created successfully! Account Number: %d\n", newAccount.accountNumber);
	printf("Starting Balance: $%.2f\n", newAccount.balance);
}
static int generateAccountNumber(void) {
	static int nextAccountNumber = 1000; 
	int candidate = nextAccountNumber;

	while  (accountNumberExists(candidate)) {
		candidate++;
	}

	nextAccountNumber = candidate + 1;
	return candidate;

}

static int accountNumberExists(int accountNumber) {
	int i;
	for (i = 0; i < accountCount; i++) {
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



	
