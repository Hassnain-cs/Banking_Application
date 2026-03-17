#pragma once
#ifndef ACCOUNT_H
#define ACCOUNT_H

// Account structure and account-related functions declarations

#include "model.h"
#ifndef  MAX_ACCOUNT
#define MAX_ACCOUNT 1000

extern Account accounts[MAX_ACCOUNT];
extern int accountCount;

#endif // ! MAX_ACCOUNT

void createAccount(void);

#define MAX_ACCOUNTS 1000

extern Account g_accounts[MAX_ACCOUNTS];
extern int g_accountCount;

int getCurrentSessionAccountNumber(void);
void logout(void);


Account* findAccountByNumber(int accountNumber);
int addAccount(const Account* account);
void createAccount(void);

int login(void);

#endif