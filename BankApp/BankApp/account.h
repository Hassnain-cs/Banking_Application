#pragma once
#ifndef ACCOUNT_H
#define ACCOUNT_H

#include "model.h"

#ifndef MAX_ACCOUNTS
#define MAX_ACCOUNTS 1000

extern Account accounts[MAX_ACCOUNTS];
extern int accountCount;
#endif // !MAX_ACCOUNTS

void createAccount(void);

#endif