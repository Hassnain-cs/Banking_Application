#pragma once
#ifndef TRANSACTION_H
#define TRANSACTION_H

#include "model.h"   

// Also expose max size
#define MAX_TRANSACTIONS 1000

// File to store all transaction history
#define TRANSACTIONS_FILE "transactions.txt"

// Make transactions accessible in other files
extern Transaction transactions[];
extern int transactionCount;

// Function declarations
void deposit(void);
void withdraw(void);
void showBalance(void);
void showTransactionHistory(void);
void saveTransactionsToFile(void);   //(IMPORTANT)

#endif