#include <stdio.h>
#include <string.h>

#include "account.h"
#include "model.h"

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



	
