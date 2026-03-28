#pragma once
#ifndef ACCOUNT_H
#define ACCOUNT_H

#include "model.h"

#define MAX_ACCOUNTS 1000

extern Account accounts[MAX_ACCOUNTS];
extern int accountCount;

void createAccount(void);
int login(void);
void logout(void);

int getCurrentSessionAccountNumber(void);

Account* findAccountByNumber(int accountNumber);
int addAccount(const Account* account);

int deleteCurrentAccount(void);

#endif