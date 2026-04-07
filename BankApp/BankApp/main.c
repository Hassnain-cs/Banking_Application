#include <stdio.h>
#include <stdlib.h>

#include "account.h"
#include "file.h"
#include "validation.h"
#include "transaction.h"
#include "transfer.h"

// Displays the main menu based on login state
static void showMenu(void);

// Pauses execution so user can read output
static void pauseForUser(void);

// Handles transaction-related options
static void transactionMenu(void);

int main(void) {

    // Load stored data at program start
    loadAccountsFromFile();
    loadTransactionsFromFile();

    int running = 1;

    while (running) {

        int loggedIn = (getCurrentSessionAccountNumber() != -1);

        showMenu(); // ✅ FIX: no argument

        int choice;

        printf("Select option: ");

        // ✅ FIXED VALIDATION (MATCHES MENU EXACTLY)
        if (!loggedIn) {
            while (scanf_s("%d", &choice) != 1 || choice < 1 || choice > 3) {
                printf("Enter a number between 1 and 3: ");
                while (getchar() != '\n');
            }
        }
        else {
            while (scanf_s("%d", &choice) != 1 || choice < 1 || choice > 5) {
                printf("Enter a number between 1 and 5: ");
                while (getchar() != '\n');
            }
        }

        while (getchar() != '\n'); // clear buffer

        // =============================
        // NOT LOGGED IN
        // =============================
        if (!loggedIn) {

            switch (choice) {

            case 1:
                createAccount();
                pauseForUser();
                break;

            case 2:
                if (!login()) {

                    char opt;
                    printf("Account not found. Create one? (Y/N): ");
                    scanf_s(" %c", &opt, 1);
                    while (getchar() != '\n');

                    if (opt == 'Y' || opt == 'y') {
                        createAccount();
                    }
                }
                pauseForUser();
                break;

            case 3:
                running = 0;

                saveAccountsToFile();
                saveTransactionsToFile();

                printf("Goodbye.\n");
                break;

            default:
                printf("Invalid choice.\n");
            }
        }

        // =============================
        // LOGGED IN
        // =============================
        else {

            switch (choice) {

            case 1:
                transactionMenu();
                break;

            case 2:
                editCurrentAccount();
                pauseForUser();
                break;

            case 3:
                deleteCurrentAccount();
                pauseForUser();
                break;

            case 4: {
                char confirm;

                printf("Are you sure you want to logout? (Y/N): ");
                scanf_s(" %c", &confirm, 1);
                while (getchar() != '\n');

                if (confirm == 'Y' || confirm == 'y') {
                    logout();
                    printf("Logged out.\n");
                }
                else {
                    printf("Logout cancelled.\n");
                }

                pauseForUser();
                break;
            }

            case 5:
                running = 0;

                saveAccountsToFile();
                saveTransactionsToFile();

                printf("Goodbye.\n");
                break;

            default:
                printf("Invalid choice.\n");
            }
        }
    }

    return 0;
}

// Displays menu cleanly depending on login state
static void showMenu(void) {
    system("cls");

    printf("=================================\n");
    printf("        BANKING SYSTEM\n");
    printf("=================================\n");

    if (getCurrentSessionAccountNumber() == -1) {
        printf("1) Create Account\n");
        printf("2) Login\n");
        printf("3) Exit\n");
    }
    else {
        printf("1) Transactions\n");
        printf("2) Edit Account\n");
        printf("3) Delete Account\n");
        printf("4) Logout\n");
        printf("5) Exit\n");
    }

    printf("=================================\n");
}

// Simple pause so user can read messages
static void pauseForUser(void) {
    printf("\nPress ENTER to continue...");
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

// Handles transaction menu navigation
static void transactionMenu(void) {

    int running = 1;

    while (running) {

        printf("\n--- TRANSACTIONS ---\n");
        printf("1) Deposit\n");
        printf("2) Withdraw\n");
        printf("3) Show Balance\n");
        printf("4) Transaction History\n");
        printf("5) Add Contact\n");
        printf("6) Show Contacts\n");
        printf("7) Remove Contact\n");
        printf("8) Transfer Money\n");
        printf("9) Back to Main Menu\n");

        int choice;

        printf("Choose an option: ");

        while (scanf_s("%d", &choice) != 1 || choice < 1 || choice > 9) {
            printf("Enter a number between 1 and 9: ");
            while (getchar() != '\n');
        }
        while (getchar() != '\n');

        switch (choice) {

        case 1: deposit(); break;
        case 2: withdraw(); break;
        case 3: showBalance(); break;
        case 4: showTransactionHistory(); break;
        case 5: addContact(); break;
        case 6: showContacts(); break;
        case 7: removeContact(); break;
        case 8: transferMoney(); break;

        case 9:
            running = 0;
            break;
        }
    }
}