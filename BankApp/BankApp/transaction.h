#pragma once
#ifndef TRANSACTION_H
#define TRANSACTION_H
#define MAX_TRANSACTIONS 1000

#include "model.h"

void deposit(void);
void withdraw(void);
void showBalance(void);
void showTransactionHistory(void);

#endif