#include <stdio.h>
#include <stdlib.h>
#include "model.h"
#include "validation.h"
#include "account.h"
#include "file.h"

extern void loadAccountsFromFile(void);
extern void loadTransactionsFromFile(void);
extern void loadContactsFromFile(void);

extern void saveAccountsToFile(void);
extern void saveTransactionsToFile(void);
extern void saveContactsToFile(void);

extern int getIntInRange(const char *prompt, int min, int max);
extern void createAccount(void);
extern int login(void);

static void showWelcomeMenu(void);
static void pauseForUser(void);

int main(void) {
    
    loadAccountsFromFile();        
    loadTransactionsFromFile();   
    loadContactsFromFile();       

    int running = 1;

    while (running) {
        showWelcomeMenu();

        
        int maxOption = (getCurrentSessionAccountNumber() != -1) ? 5 : 3;
        int choice = getIntInRange("Select an option: ", 1, maxOption);;

        switch (choice) {
        case 1: {
            
            createAccount();   
            pauseForUser();
            break;
        }
        case 2: {

            if (login()) {
                printf("\nLogin successful. Account #%d\n",
                    getCurrentSessionAccountNumber());
            }
            else {
                printf("\nLogin failed.\n");
            }
            break;
        }
        case 3: {
            running = 0;

            saveAccountsToFile();
            saveTransactionsToFile();
            saveContactsToFile();

            printf("\nGoodbye.\n");
            break;
        }

        case 4: {
            if (getCurrentSessionAccountNumber() == -1) {
                printf("\nYou must login first.\n");
            }
            else {
                deleteCurrentAccount(); 
            }
            pauseForUser();
            break;
        }
        case 5: {
            if (getCurrentSessionAccountNumber() == -1) {
                printf("\nYou are not logged in.\n");
            }
            else {
                logout();
                printf("\nLogged out.\n");
            }
            pauseForUser();
            break;
        }

        default:

            printf("\nInvalid option.\n");
            pauseForUser();
            break;
        }
    }

    return 0;
}

    static void showWelcomeMenu(void) {
    (void)system("cls"); 
    printf("=====================================\n");
    printf("         BANKING SYSTEM          \n");
    printf("=====================================\n");
    printf("1) Create Account\n");
    printf("2) Login\n");
    printf("3) Exit\n");
    if (getCurrentSessionAccountNumber() != -1) {
        printf("4) Delete My Account\n");
        printf("5) Logout\n");
    }
    printf("=====================================\n");
}

static void pauseForUser(void) {
    printf("\nPress ENTER to continue...");
    int c;
    while ((c = getchar()) != '\n' && c != EOF) { }
   (void)getchar();
}