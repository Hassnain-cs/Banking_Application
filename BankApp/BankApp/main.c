#include <stdio.h>
#include <stdlib.h>

#include "account.h"
#include "file.h"
#include "validation.h"
#include "transaction.h"

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

        showMenu();

        int loggedIn = (getCurrentSessionAccountNumber() != -1);
        int choice;

        printf("Select option: ");

        // Input validation depending on login state
        if (!loggedIn) {
            while (scanf_s("%d", &choice) != 1 || choice < 1 || choice > 3) {
                printf("Enter a number between 1 and 3: ");
                while (getchar() != '\n');
            }
        }
        else {
            while (scanf_s("%d", &choice) != 1 || choice < 1 || choice > 4) {
                printf("Enter a number between 1 and 4: ");
                while (getchar() != '\n');
            }
        }
        while (getchar() != '\n'); // clear buffer

        switch (choice) {

            // =========================
            // NOT LOGGED IN OPTIONS
            // =========================
        case 1:
            if (!loggedIn) {
                createAccount();
            }
            else {
                transactionMenu(); // Option 1 becomes Transactions when logged in
            }
            pauseForUser();
            break;

        case 2:
            if (!loggedIn) {
                if (!login()) {

                    char opt;
                    printf("Account not found. Create one? (Y/N): ");
                    scanf_s(" %c", &opt, 1);
                    while (getchar() != '\n');

                    if (opt == 'Y' || opt == 'y') {
                        createAccount();
                    }
                }
            }
            else {
                deleteCurrentAccount(); // Option 2 becomes Delete Account
            }
            pauseForUser();
            break;

        case 3:
            if (!loggedIn) {
                running = 0;

                // Save before exit
                saveAccountsToFile();
                saveTransactionsToFile();

                printf("Goodbye.\n");
            }
            else {
                logout();
                printf("Logged out.\n");
            }
            pauseForUser();
            break;

        case 4:
            if (loggedIn) {
                running = 0;

                // Save before exit
                saveAccountsToFile();
                saveTransactionsToFile();

                printf("Goodbye.\n");
            }
            pauseForUser();
            break;
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
        printf("2) Delete Account\n");
        printf("3) Logout\n");
        printf("4) Exit\n");
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
        printf("5) Back to Main Menu\n");

        int choice;

        printf("Choose an option: ");

        while (scanf_s("%d", &choice) != 1 || choice < 1 || choice > 5) {
            printf("Enter a number between 1 and 5: ");
            while (getchar() != '\n');
        }
        while (getchar() != '\n');

        switch (choice) {

        case 1:
            deposit();
            break;

        case 2:
            withdraw();
            break;

        case 3:
            showBalance();
            break;

        case 4:
            showTransactionHistory();
            break;

        case 5:
            running = 0; // Exit transaction menu
            break;
        }
    }
}