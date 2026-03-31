#include <stdio.h>
#include <stdlib.h>

#include "account.h"
#include "file.h"
#include "validation.h"
#include "transaction.h"

static void showMenu(void);
static void pauseForUser(void);
static void transactionMenu(void);

int main(void) {

    loadAccountsFromFile();
    loadTransactionsFromFile();

    int running = 1;

    while (running) {
        showMenu();

        int loggedIn = (getCurrentSessionAccountNumber() != -1);
        int choice;

        if (!loggedIn) {
            printf("Select option: ");
            while (scanf_s("%d", &choice) != 1 || choice < 1 || choice > 3) {
                printf("Enter a number between 1 and 3: ");
                while (getchar() != '\n');
            }
            while (getchar() != '\n');
        }
        else {
            printf("Select option: ");
            while (scanf_s("%d", &choice) != 1 || choice < 1 || choice > 5) {
                printf("Enter a number between 1 and 5: ");
                while (getchar() != '\n');
            }
            while (getchar() != '\n');
        }

        switch (choice) {

        case 1:
            if (!loggedIn) createAccount();
            else printf("Already logged in.\n");
            pauseForUser();
            break;

        case 2:
            if (!loggedIn) {
                if (!login()) {
                    int opt;
                    printf("Account not found. Create one? (1=Yes / 0=No): ");
                    while (scanf_s("%d", &opt) != 1 || (opt != 0 && opt != 1)) {
                        printf("Enter 0 or 1: ");
                        while (getchar() != '\n');
                    }
                    while (getchar() != '\n');
                    if (opt == 1) createAccount();
                }
            }
            else {
                printf("Already logged in.\n");
            }
            pauseForUser();
            break;

        case 3:
            if (!loggedIn) {
                running = 0;
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
                deleteCurrentAccount();
            }
            pauseForUser();
            break;

        case 5:
            if (loggedIn) {
                transactionMenu();
            }
            pauseForUser();
            break;
        }
    }

    return 0;
}

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
        printf("1) (Disabled - Already Logged In)\n");
        printf("2) (Disabled - Already Logged In)\n");
        printf("3) Logout\n");
        printf("4) Delete Account\n");
        printf("5) Transactions\n");
    }

    printf("=================================\n");
}

static void pauseForUser(void) {
    printf("\nPress ENTER to continue...");
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

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
            running = 0;
            break;
        }
    }
}