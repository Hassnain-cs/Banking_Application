#pragma once
#ifndef MODELS_H
#define MODELS_H

#include <time.h>

#define MAX_NAME        25
#define MAX_PASSWORD    16
#define MAX_EMAIL       50
#define MAX_PHONE       20
#define MAX_FIELD       50
#define MAX_NICKNAME    16

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

    int birthDay;
    int birthMonth;
    int birthYear;

    char email[MAX_EMAIL];
    char phone[MAX_PHONE];

    // Structured Address
    char streetNumber[MAX_FIELD];
    char streetName[MAX_FIELD];
    char city[MAX_FIELD];
    char country[MAX_FIELD];
    char postalCode[MAX_FIELD];

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