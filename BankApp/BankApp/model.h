/*
 * @file model.h
 * @brief Defines core data structures for the banking application
 */

#pragma once
#ifndef MODELS_H
#define MODELS_H

#include <time.h>

 /* ===============================================================
  * CONSTANT DEFINITIONS
  * =============================================================== */

#define MAX_NAME        25    /* Maximum length for first/last names */
#define MAX_PASSWORD    16    /* Maximum length for password */
#define MAX_EMAIL       50    /* Maximum length for email address */
#define MAX_PHONE       20    /* Maximum length for phone number */
#define MAX_FIELD       50    /* Maximum length for address fields */
#define MAX_NICKNAME    16    /* Maximum length for contact nickname */


  /* ===============================================================
   * ENUMERATION: Transaction Types
   * =============================================================== */

   /*
    * @enum TransactionType
    * @brief Identifies the type of financial transaction
    */

typedef enum {
    TX_DEPOSIT = 1,      /**< Money added to account */
    TX_WITHDRAW,         /**< Money removed from account */
    TX_TRANSFER_OUT,     /**< Money sent to another user */
    TX_TRANSFER_IN       /**< Money received from another user */
} TransactionType;

/* ===============================================================
 * STRUCTURE: Account
 * =============================================================== */

 /*
  * @struct Account
  * @brief Stores all information for a single bank account
  *
  * Contains personal information, address details, authentication data,
  * and financial state for a single user account.
  */
typedef struct {
    // IDENTIFICATION
    int  accountNumber;       //Unique identifier (auto-generated, starts at 1000)

    // PERSONAL INFORMATION
    char firstName[MAX_NAME];        // User's first name
    char lastName[MAX_NAME];         // User's last name

    // DATE OF BIRTH
    int birthDay;                    // Day of birth (1-31)
    int birthMonth;                  // Month of birth (1-12)
    int birthYear;                   // Year of birth (1900+)

    // CONTACT INFORMATION
    char email[MAX_EMAIL];           // Email address (must contain @ and .)
    char phone[MAX_PHONE];           // Phone number (exactly 10 digits)

    // ADDRESS INFORMATION
    char streetNumber[MAX_FIELD];    // House/building number (numeric only)
    char streetName[MAX_FIELD];      // Street name
    char city[MAX_FIELD];            // City name
    char province[MAX_FIELD];        // Province or state (e.g., Ontario, Quebec)
    char country[MAX_FIELD];         // Country name
    char postalCode[MAX_FIELD];      // Canadian postal code (format: A1A1A1)

    // AUTHENTICATION
    char password[MAX_PASSWORD];     // User password (min 6 chars, letters + numbers)

    // FINANCIAL
    double balance;                  // Current account balance (initial: $100.00)

    // STATUS
    int  isActive;                   // 1 = active, 0 = soft-deleted
} Account;

/* ===============================================================
 * STRUCTURE: Contact
 * =============================================================== */

 /**
  * @struct Contact
  * @brief Stores a contact relationship between two accounts
  */
typedef struct {
    int ownerAccountNumber;          // Account that owns this contact list entry
    int contactAccountNumber;        // Account number of the contact
    char nickname[MAX_NICKNAME];     // User-defined nickname for the contact
} Contact;

/* ===============================================================
 * STRUCTURE: Transaction
 * =============================================================== */

 /*
  * @struct Transaction
  * @brief Records a single financial transaction
  */
typedef struct {
    int ownerAccountNumber;          // Account that this transaction belongs to
    TransactionType type;            // Type of transaction (deposit, withdraw, transfer)
    double amount;                   // Transaction amount (always positive)
    time_t timestamp;                // When transaction occurred (Unix timestamp)
    int otherAccountNumber;          // For transfers: the other account involved
} Transaction;

#endif