#pragma once
#ifndef ACCOUNT_H
#define ACCOUNT_H

#include "model.h"   

#define MAX_ACCOUNTS 1000

extern Account g_accounts[MAX_ACCOUNTS];
extern int g_accountCount;


int getCurrentSessionAccountNumber(void);
void logout(void);


Account* findAccountByNumber(int accountNumber);


int deleteAccountByNumber(int accountNumber); 
int deleteCurrentAccount(void);              

#endif
