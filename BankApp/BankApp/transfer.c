#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <time.h>

#include "transfer.h"
#include "account.h"
#include "transaction.h"

extern Transaction transactions[];
extern int transactionCount;

#define MAX_CONTACTS 500

// Stores all contacts in memory
static Contact contacts[MAX_CONTACTS];
static int contactCount = 0;

/*
Checks if a contact already exists for a user.
Returns 1 if exists, 0 if not found.
*/
static int contactExists(int owner, int target) {
    for (int i = 0; i < contactCount; i++) {
        if (contacts[i].ownerAccountNumber == owner &&
            contacts[i].contactAccountNumber == target) {
            return 1;
        }
    }
    return 0;
}

/*
Adds a new contact for the logged-in user.
Requires a valid existing account number.
*/
void addContact(void) {

    int current = getCurrentSessionAccountNumber();
    if (current == -1) {
        printf("No account logged in.\n");
        return;
    }

    int target;

    printf("Enter account number to add (0 to cancel): ");

    if (scanf_s("%d", &target) != 1) {
        while (getchar() != '\n');
        printf("Invalid input.\n");
        return;
    }
    while (getchar() != '\n');

    if (target == 0) {
        printf("Operation cancelled.\n");
        return;
    }

    if (target == current) {
        printf("You cannot add yourself.\n");
        return;
    }

    Account* acc = findAccountByNumber(target);
    if (!acc) {
        printf("Account does not exist.\n");
        return;
    }

    if (contactExists(current, target)) {
        printf("Contact already exists.\n");
        return;
    }

    if (contactCount >= MAX_CONTACTS) {
        printf("Contact list full.\n");
        return;
    }

    Contact c;
    c.ownerAccountNumber = current;
    c.contactAccountNumber = target;

    printf("Enter nickname for this contact: ");
    fgets(c.nickname, sizeof(c.nickname), stdin);
    c.nickname[strcspn(c.nickname, "\n")] = 0;

    contacts[contactCount++] = c;

    printf("Contact added successfully.\n");
}

/*
Displays all contacts for current user
*/
void showContacts(void) {

    int current = getCurrentSessionAccountNumber();
    if (current == -1) return;

    int found = 0;

    printf("\n--- CONTACTS ---\n");

    for (int i = 0; i < contactCount; i++) {
        if (contacts[i].ownerAccountNumber == current) {

            printf("Account: %d | Nickname: %s\n",
                contacts[i].contactAccountNumber,
                contacts[i].nickname);

            found = 1;
        }
    }

    if (!found) {
        printf("No contacts found.\n");
    }
}

/*
Removes a contact for current user
*/
void removeContact(void) {

    int current = getCurrentSessionAccountNumber();
    if (current == -1) return;

    int target;

    printf("Enter account number to remove (0 to cancel): ");

    if (scanf_s("%d", &target) != 1) {
        while (getchar() != '\n');
        printf("Invalid input.\n");
        return;
    }
    while (getchar() != '\n');

    if (target == 0) {
        printf("Operation cancelled.\n");
        return;
    }

    for (int i = 0; i < contactCount; i++) {

        if (contacts[i].ownerAccountNumber == current &&
            contacts[i].contactAccountNumber == target) {

            for (int j = i; j < contactCount - 1; j++) {
                contacts[j] = contacts[j + 1];
            }

            contactCount--;
            printf("Contact removed successfully.\n");
            return;
        }
    }

    printf("Contact not found.\n");
}

/*
Transfers money between two accounts
*/
void transferMoney(void) {

    int senderNum = getCurrentSessionAccountNumber();
    if (senderNum == -1) return;

    Account* sender = findAccountByNumber(senderNum);
    if (!sender) return;

    int receiverNum;

    printf("Enter receiver account number (0 to cancel): ");

    if (scanf_s("%d", &receiverNum) != 1) {
        while (getchar() != '\n');
        printf("Invalid input.\n");
        return;
    }
    while (getchar() != '\n');

    if (receiverNum == 0) {
        printf("Transfer cancelled.\n");
        return;
    }

    if (receiverNum == senderNum) {
        printf("Cannot transfer to yourself.\n");
        return;
    }

    Account* receiver = findAccountByNumber(receiverNum);
    if (!receiver) {
        printf("Receiver not found.\n");
        return;
    }

    double amount;

    while (1) {
        printf("Enter amount (0 to cancel): ");

        if (scanf_s("%lf", &amount) != 1) {
            while (getchar() != '\n');
            printf("Invalid input.\n");
            continue;
        }
        while (getchar() != '\n');

        if (amount == 0) {
            printf("Transfer cancelled.\n");
            return;
        }

        if (amount < 0) {
            printf("Enter positive amount.\n");
            continue;
        }

        if (amount > sender->balance) {
            printf("Insufficient balance.\n");
            continue;
        }

        break;
    }

    sender->balance -= amount;
    receiver->balance += amount;

    // OUTGOING transaction
    Transaction t1;
    t1.ownerAccountNumber = senderNum;
    t1.type = TX_TRANSFER_OUT;
    t1.amount = amount;
    t1.timestamp = time(NULL);
    t1.otherAccountNumber = receiverNum;

    // INCOMING transaction
    Transaction t2;
    t2.ownerAccountNumber = receiverNum;
    t2.type = TX_TRANSFER_IN;
    t2.amount = amount;
    t2.timestamp = time(NULL);
    t2.otherAccountNumber = senderNum;

    if (transactionCount < MAX_TRANSACTIONS) {
        transactions[transactionCount++] = t1;
        transactions[transactionCount++] = t2;
    }

    // IMPROVED OUTPUT (NAME + ACCOUNT NUMBER)
    printf("Transfer successful: $%.2lf sent to %s %s (Acc: %d)\n",
        amount,
        receiver->firstName,
        receiver->lastName,
        receiverNum);
}



