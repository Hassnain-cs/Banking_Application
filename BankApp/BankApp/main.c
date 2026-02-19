#include <stdio.h>
#include <stdlib.h>
#include "model.h"
#include "validation.h"
#include "account.h"
#include "file.h"

// External module function prototypes (prevent implicit-declaration warnings)
extern void loadAccountsFromFile(void);
extern void loadTransactionsFromFile(void);
extern void loadContactsFromFile(void);

extern void saveAccountsToFile(void);
extern void saveTransactionsToFile(void);
extern void saveContactsToFile(void);

extern int getIntInRange(const char *prompt, int min, int max);
extern void createAccount(void);
extern int login(void);

// ---------- Prototypes ----------
static void showWelcomeMenu(void);
static void pauseForUser(void);

int main(void) {
    // 1) Load data at start (file module)
    // If your file module uses different function names, update these calls.
    loadAccountsFromFile();        
    loadTransactionsFromFile();   
    loadContactsFromFile();       

    int running = 1;

    while (running) {
        showWelcomeMenu();

        // 2) Safe menu choice (validation module)
        // Use your own validation function name if different.
        int choice = getIntInRange("Select an option: ", 1, 3);

        switch (choice) {
        case 1: {
            // Create Account
            createAccount();   // account module
            pauseForUser();
            break;
        }
        case 2: {
            // Login
            // Recommended: login returns either:
            // - accountNumber (int) on success
            // - -1 on failure
            int accNum = login();
            if (accNum != -1) {
                // After login, you will call the logged-in menu (later issue)
                // For now, just confirm success:
                printf("\nLogin successful. Account #%d\n", accNum);
            }
            else {
                printf("\nLogin failed.\n");
            }
            pauseForUser();
            break;
        }
        case 3: {
            // Exit
            running = 0;

            // 3) Save data at exit
            saveAccountsToFile();
            saveTransactionsToFile();
            saveContactsToFile();

            printf("\nGoodbye.\n");
            break;
        }
        default:
            // Should never happen because getIntInRange blocks invalid
            printf("\nInvalid option.\n");
            pauseForUser();
            break;
        }
    }

    return 0;
}

static void showWelcomeMenu(void) {
    (void)system("cls"); // Windows. If needed for mac/linux use "clear".
    printf("=====================================\n");
    printf("         BANKING SYSTEM (C)          \n");
    printf("=====================================\n");
    printf("1) Create Account\n");
    printf("2) Login\n");
    printf("3) Exit\n");
    printf("=====================================\n");
}

static void pauseForUser(void) {
    printf("\nPress ENTER to continue...");
    int c;
    /* clear leftover input */
    while ((c = getchar()) != '\n' && c != EOF) { }
    /* consume the newline (if any) */
    (void)getchar();
}