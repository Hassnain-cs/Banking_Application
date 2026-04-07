#pragma once
#ifndef MODELS_H
#define MODELS_H

#include <time.h>

#define MAX_NAME        25
#define MAX_PASSWORD    16
#define MAX_NICKNAME    16
#define MAX_ADDRESS     50
#define MAX_EMAIL       50
#define MAX_PHONE       15
#define MAX_POSTAL      10

typedef enum {
    TX_DEPOSIT = 1,
    TX_WITHDRAW,
    TX_TRANSFER_OUT,
    TX_TRANSFER_IN
} TransactionType;

typedef struct {
    int  accountNumber;

    char firstName[MAX_NAME];
    char lastName[MAX_NAME];

    int  birthDay;
    int  birthMonth;
    int  birthYear;

    char streetNumber[10];
    char streetName[MAX_ADDRESS];
    char city[MAX_ADDRESS];
    char country[MAX_ADDRESS];
    char postalCode[MAX_POSTAL];

    char email[MAX_EMAIL];
    char phone[MAX_PHONE];

    char password[MAX_PASSWORD];
    double balance;
    int  isActive;

} Account;

typedef struct {
    int ownerAccountNumber;
    int contactAccountNumber;
    char nickname[MAX_NICKNAME];
} Contact;

typedef struct {
    int ownerAccountNumber;
    TransactionType type;
    double amount;
    time_t timestamp;
    int otherAccountNumber;
} Transaction;

#endif