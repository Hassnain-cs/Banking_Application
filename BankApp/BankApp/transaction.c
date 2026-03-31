#include <stdio.h>
#include <time.h>
#include "transaction.h"
#include "account.h"
#include "validation.h"

#define MAX_TRANSACTIONS 1000

static Transaction transactions[MAX_TRANSACTIONS];
static int transactionCount = 0;

void deposit(void) {
    int accNum = getCurrentSessionAccountNumber();
    if (accNum == -1) return;

    Account* acc = findAccountByNumber(accNum);
    if (!acc) return;

    double amount;
    printf("Enter amount to deposit: ");
    while (scanf_s("%lf", &amount) != 1 || amount <= 0) {
        printf("Enter a positive number: ");
        while (getchar() != '\n'); // clear buffer
    }
    while (getchar() != '\n');

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
}

void withdraw(void) {
    int accNum = getCurrentSessionAccountNumber();
    if (accNum == -1) return;

    Account* acc = findAccountByNumber(accNum);
    if (!acc) return;

    double amount;
    printf("Enter amount to withdraw: ");
    while (scanf_s("%lf", &amount) != 1 || amount <= 0 || amount > acc->balance) {
        if (amount <= 0) printf("Enter a positive number: ");
        else printf("Insufficient balance. Enter smaller amount: ");
        while (getchar() != '\n');
    }
    while (getchar() != '\n');

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
}

void showBalance(void) {
    int accNum = getCurrentSessionAccountNumber();
    if (accNum == -1) return;

    Account* acc = findAccountByNumber(accNum);
    if (!acc) return;

    printf("Current balance: $%.2lf\n", acc->balance);
}

void showTransactionHistory(void) {
    int accNum = getCurrentSessionAccountNumber();
    if (accNum == -1) return;

    printf("Transaction History:\n");
    for (int i = 0; i < transactionCount; i++) {
        if (transactions[i].ownerAccountNumber == accNum) {
            char* typeStr = transactions[i].type == TX_DEPOSIT ? "Deposit" : "Withdraw";
            printf("%s: $%.2lf\n", typeStr, transactions[i].amount);
        }
    }
}