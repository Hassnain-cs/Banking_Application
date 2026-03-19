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

        
        int choice = getIntInRange("Select an option: ", 1, 3);

        switch (choice) {
        case 1: {
            
            createAccount();   
            pauseForUser();
            break;
        }
        case 2: {
            
            int accNum = login();
            if (accNum != -1) {
                
                printf("\nLogin successful. Account #%d\n", accNum);
            }
            else {
                printf("\nLogin failed.\n");
            }
            pauseForUser();
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
    printf("=====================================\n");
}

static void pauseForUser(void) {
    printf("\nPress ENTER to continue...");
    int c;
    while ((c = getchar()) != '\n' && c != EOF) { }
   (void)getchar();
}