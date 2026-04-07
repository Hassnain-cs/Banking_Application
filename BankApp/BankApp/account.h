#pragma once
#ifndef ACCOUNT_H
#define ACCOUNT_H
// File where all accounts will be stored
#define ACCOUNTS_FILE "accounts.txt"

#include "model.h"

#define MAX_ACCOUNTS 1000

void saveAccountsToFile(void);

extern Account accounts[MAX_ACCOUNTS];
extern int accountCount;

void createAccount(void);
int login(void);
void editCurrentAccount(void);
void logout(void);

int getCurrentSessionAccountNumber(void);

Account* findAccountByNumber(int accountNumber);
int addAccount(const Account* account);

int deleteCurrentAccount(void);

#endif