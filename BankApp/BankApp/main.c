#include <stdio.h>
#include <stdlib.h>

#include "account.h"
#include "file.h"
#include "validation.h"

static void showMenu(void);
static void pauseForUser(void);

int main(void) {

    loadAccountsFromFile();

    int running = 1;

    while (running) {
        showMenu();

        int loggedIn = (getCurrentSessionAccountNumber() != -1);

        int choice = loggedIn
            ? getIntInRange("Select option: ", 1, 4)
            : getIntInRange("Select option: ", 1, 3);

        switch (choice) {

        case 1:
            if (!loggedIn) createAccount();
            else printf("Already logged in.\n");
            pauseForUser();
            break;

        case 2:
            if (!loggedIn) {
                if (!login()) {
                    printf("Account not found. Create one? (1=Yes / 0=No): ");
                    int opt; scanf("%d", &opt); getchar();
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
    }

    printf("=================================\n");
}

static void pauseForUser(void) {
    printf("\nPress ENTER to continue...");
    while (getchar() != '\n');
}