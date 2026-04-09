#define _CRT_SECURE_NO_WARNINGS
#include "pch.h"
#include "CppUnitTest.h"

#include <stdio.h>

extern "C" {
#include "../BankApp/account.h"
    void saveAccountsToFile(void);
    void loadAccountsFromFile(void);
    void fillTestAccount(Account& acc, int accNum, double balance, int active)
    {
        memset(&acc, 0, sizeof(Account));

        acc.accountNumber = accNum;

        strcpy_s(acc.firstName, "John");
        strcpy_s(acc.lastName, "Doe");

        acc.birthDay = 1;
        acc.birthMonth = 1;
        acc.birthYear = 2000;

        strcpy_s(acc.streetNumber, "123");
        strcpy_s(acc.streetName, "Main");
        strcpy_s(acc.city, "Waterloo");
        strcpy_s(acc.country, "Canada");
        strcpy_s(acc.postalCode, "A1A1A1");

        strcpy_s(acc.password, "pass123");

        acc.balance = balance;
        acc.isActive = active;
    }

    extern Account accounts[];
    extern int accountCount;
}

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace BankingTests
{
    TEST_CLASS(FileTests)
    {
    public:

        // Helper to reset state
        void resetData()
        {
            accountCount = 0;
        }

        // SAVE + LOAD SINGLE ACCOUNT
        TEST_METHOD(File_SaveLoad_SingleAccount)
        {
            resetData();

            
            Account acc = {};

            acc.accountNumber = 1001;

            strcpy_s(acc.firstName, "John");
            strcpy_s(acc.lastName, "Doe");

            acc.birthDay = 1;
            acc.birthMonth = 1;
            acc.birthYear = 2000;

            strcpy_s(acc.streetNumber, "123");
            strcpy_s(acc.streetName, "Main");
            strcpy_s(acc.city, "Waterloo");
            strcpy_s(acc.country, "Canada");
            strcpy_s(acc.postalCode, "A1A1A1");

            strcpy_s(acc.password, "pass123");

            acc.balance = 500.0;
            acc.isActive = 1;

            accounts[0] = acc;
            accountCount = 1;

            saveAccountsToFile();

            // CLEAR MEMORY
            accountCount = 0;

            loadAccountsFromFile();

            Assert::AreEqual(1, accountCount);
            Assert::AreEqual(1001, accounts[0].accountNumber);
            Assert::AreEqual(500.0, accounts[0].balance);
        }

        // MULTIPLE ACCOUNTS
        TEST_METHOD(File_SaveLoad_MultipleAccounts)
        {
            accountCount = 0;

            fillTestAccount(accounts[0], 1000, 100.0, 1);
            fillTestAccount(accounts[1], 1001, 200.0, 1);
            fillTestAccount(accounts[2], 1002, 300.0, 1);

            accountCount = 3;

            saveAccountsToFile();

            accountCount = 0;

            loadAccountsFromFile();

            Assert::AreEqual(3, accountCount);
            Assert::AreEqual(1000, accounts[0].accountNumber);
            Assert::AreEqual(200.0, accounts[1].balance);
            Assert::AreEqual(300.0, accounts[2].balance);
        }

        // EMPTY FILE
        TEST_METHOD(File_EmptyFile)
        {
            resetData();

            saveAccountsToFile();

            accountCount = 5; // fake dirty data

            loadAccountsFromFile();

            Assert::AreEqual(0, accountCount);
        }

        // DATA PERSISTENCE AFTER MODIFY
        TEST_METHOD(File_DataPersistence)
        {
            accountCount = 0;

            fillTestAccount(accounts[0], 1111, 100.0, 1);
            accountCount = 1;

            saveAccountsToFile();

            // Change AFTER saving
            accounts[0].balance = 999.0;

            loadAccountsFromFile();

            // Should go back to saved value
            Assert::AreEqual(100.0, accounts[0].balance);
        }

        // INACTIVE ACCOUNT
        TEST_METHOD(File_InactiveAccount)
        {
            accountCount = 0;

            fillTestAccount(accounts[0], 2001, 300.0, 0); // inactive
            accountCount = 1;

            saveAccountsToFile();

            accountCount = 0;

            loadAccountsFromFile();

            Assert::AreEqual(1, accountCount);
            Assert::AreEqual(0, accounts[0].isActive);
        }
    };
}