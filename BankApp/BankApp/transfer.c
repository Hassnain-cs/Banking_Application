#include <stdio.h>
#include <string.h>

#include "transfer.h"
#include "account.h"
#include "validation.h"

#define MAX_CONTACTS 500

// Stores all contacts
static Contact contacts[MAX_CONTACTS];
static int contactCount = 0;

// Checks if contact already exists
static int contactExists(int ownerAcc, int contactAcc) {
    for (int i = 0; i < contactCount; i++) {
        if (contacts[i].ownerAccountNumber == ownerAcc &&
            contacts[i].contactAccountNumber == contactAcc) {
            return 1;
        }
    }
    return 0;
}

// Adds a new contact with full validation
void addContact(void) {

    int currentAcc = getCurrentSessionAccountNumber();

    if (currentAcc == -1) {
        printf("No account logged in.\n");
        return;
    }

    int targetAcc;

    printf("Enter account number to add as contact (0 to cancel): ");

    // Input validation loop
    while (scanf_s("%d", &targetAcc) != 1) {
        printf("Invalid input. Enter a valid account number: ");
        while (getchar() != '\n');
    }
    while (getchar() != '\n');

    // Allow user to cancel operation
    if (targetAcc == 0) {
        printf("Operation cancelled.\n");
        return;
    }

    // Prevent adding self
    if (targetAcc == currentAcc) {
        printf("You cannot add yourself as a contact.\n");
        return;
    }

    // Check if account exists
    Account* acc = findAccountByNumber(targetAcc);
    if (!acc) {
        printf("Account does not exist.\n");
        return;
    }

    // Prevent duplicate contacts
    if (contactExists(currentAcc, targetAcc)) {
        printf("Contact already exists.\n");
        return;
    }

    // Check storage limit
    if (contactCount >= MAX_CONTACTS) {
        printf("Contact list is full.\n");
        return;
    }

    Contact c;
    c.ownerAccountNumber = currentAcc;
    c.contactAccountNumber = targetAcc;

    printf("Enter nickname for contact: ");
    fgets(c.nickname, sizeof(c.nickname), stdin);
    c.nickname[strcspn(c.nickname, "\n")] = 0;

    contacts[contactCount++] = c;

    printf("Contact added successfully.\n");
}

// Displays all contacts for current user
void showContacts(void) {

    int currentAcc = getCurrentSessionAccountNumber();

    if (currentAcc == -1) {
        printf("No account logged in.\n");
        return;
    }

    printf("\n--- YOUR CONTACTS ---\n");

    int found = 0;

    for (int i = 0; i < contactCount; i++) {
        if (contacts[i].ownerAccountNumber == currentAcc) {
            printf("Account: %d | Nickname: %s\n",
                contacts[i].contactAccountNumber,
                contacts[i].nickname);
            found = 1;
        }
    }

    if (!found) {
        printf("No contacts saved.\n");
    }
}