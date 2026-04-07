#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <time.h>
#include "transaction.h"
#include "account.h"

#define MAX_TRANSACTIONS 1000

// Stores all transactions performed in the system
static Transaction transactions[MAX_TRANSACTIONS];
static int transactionCount = 0;

/*
Handles depositing money and shows updated balance
*/
void deposit(void) {

    int accNum = getCurrentSessionAccountNumber();
    if (accNum == -1) return;

    Account* acc = findAccountByNumber(accNum);
    if (!acc) return;

    double amount;

    while (1) {
        printf("Enter amount to deposit (0 to cancel): ");

        if (scanf_s("%lf", &amount) != 1) {
            printf("Invalid input.\n");
            while (getchar() != '\n');
            continue;
        }
        while (getchar() != '\n');

        if (amount == 0) {
            printf("Deposit cancelled.\n");
            return;
        }

        if (amount < 0) {
            printf("Enter a positive amount.\n");
            continue;
        }

        break;
    }

    acc->balance += amount;

    Transaction t;
    t.ownerAccountNumber = accNum;
    t.type = TX_DEPOSIT;
    t.amount = amount;
    t.timestamp = time(NULL);
    t.otherAccountNumber = 0;

    if (transactionCount < MAX_TRANSACTIONS) {
        transactions[transactionCount++] = t;
    }

    printf("Deposited $%.2lf successfully.\n", amount);
    printf("Updated Balance: $%.2lf\n", acc->balance);
}

/*
Handles withdrawal and shows updated balance
*/
void withdraw(void) {

    int accNum = getCurrentSessionAccountNumber();
    if (accNum == -1) return;

    Account* acc = findAccountByNumber(accNum);
    if (!acc) return;

    if (acc->balance == 0) {
        printf("Your balance is $0. Withdrawal not possible.\n");
        return;
    }

    double amount;

    while (1) {
        printf("Enter amount to withdraw (0 to cancel): ");

        if (scanf_s("%lf", &amount) != 1) {
            printf("Invalid input.\n");
            while (getchar() != '\n');
            continue;
        }
        while (getchar() != '\n');

        if (amount == 0) {
            printf("Withdrawal cancelled.\n");
            return;
        }

        if (amount < 0) {
            printf("Enter a positive amount.\n");
            continue;
        }

        if (amount > acc->balance) {
            printf("Insufficient balance. Available: $%.2lf\n", acc->balance);
            continue;
        }

        break;
    }

    acc->balance -= amount;

    Transaction t;
    t.ownerAccountNumber = accNum;
    t.type = TX_WITHDRAW;
    t.amount = amount;
    t.timestamp = time(NULL);
    t.otherAccountNumber = 0;

    if (transactionCount < MAX_TRANSACTIONS) {
        transactions[transactionCount++] = t;
    }

    printf("Withdrew $%.2lf successfully.\n", amount);
    printf("Updated Balance: $%.2lf\n", acc->balance);
}

/*
Displays current account balance
*/
void showBalance(void) {

    int accNum = getCurrentSessionAccountNumber();
    if (accNum == -1) return;

    Account* acc = findAccountByNumber(accNum);
    if (!acc) return;

    printf("Current balance: $%.2lf\n", acc->balance);
}

/*
Displays transaction history
*/
void showTransactionHistory(void) {

    int accNum = getCurrentSessionAccountNumber();
    if (accNum == -1) return;

    printf("Transaction History:\n");

    for (int i = 0; i < transactionCount; i++) {

        if (transactions[i].ownerAccountNumber == accNum) {

			Account* other = findAccountByNumber(transactions[i].otherAccountNumber);

            const char* typeStr;

            if (transactions[i].type == TX_DEPOSIT)
                typeStr = "Deposit";
            else if (transactions[i].type == TX_WITHDRAW)
                typeStr = "Withdraw";
            else if (transactions[i].type == TX_TRANSFER_OUT)
                typeStr = "Transfer Sent";
            else
                typeStr = "Transfer Received";

            if (other) {
                printf("%s: $%.2lf (%s %s - %d)\n",
                    typeStr,
                    transactions[i].amount,
                    other->firstName,
                    other->lastName,
                    transactions[i].otherAccountNumber);

            printf("%s: $%.2lf\n", typeStr, transactions[i].amount);
        }
    }
}