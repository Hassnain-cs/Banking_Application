#include "pch.h"
#include "CppUnitTest.h"

extern "C" {
#include "../BankApp/account.h"
    void depositToAccount(Account* acc, double amount);
}

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace BankingTests
{
    TEST_CLASS(DepositTests)
    {
    public:

        // VALID DEPOSIT
        TEST_METHOD(Deposit_Valid)
        {
            Account acc;
            acc.balance = 100.0;

            depositToAccount(&acc, 50.0);

            Assert::AreEqual(150.0, acc.balance);
        }

        // ZERO AMOUNT (should not change balance)
        TEST_METHOD(Deposit_Zero)
        {
            Account acc;
            acc.balance = 100.0;

            depositToAccount(&acc, 0.0);

            Assert::AreEqual(100.0, acc.balance);
        }

        // NEGATIVE AMOUNT (should not change balance)
        TEST_METHOD(Deposit_Negative)
        {
            Account acc;
            acc.balance = 100.0;

            depositToAccount(&acc, -50.0);

            Assert::AreEqual(100.0, acc.balance);
        }

        // LARGE AMOUNT
        TEST_METHOD(Deposit_LargeAmount)
        {
            Account acc;
            acc.balance = 100.0;

            depositToAccount(&acc, 1000000.0);

            Assert::AreEqual(1000100.0, acc.balance);
        }

        // NULL ACCOUNT (should not crash)
        TEST_METHOD(Deposit_NullAccount)
        {
            Account* acc = nullptr;

            // Should safely return without crash
            depositToAccount(acc, 100.0);

            Assert::IsTrue(true); // test passes if no crash
        }

        // MULTIPLE DEPOSITS
        TEST_METHOD(Deposit_MultipleTransactions)
        {
            Account acc;
            acc.balance = 100.0;

            depositToAccount(&acc, 50.0);
            depositToAccount(&acc, 25.0);
            depositToAccount(&acc, 25.0);

            Assert::AreEqual(200.0, acc.balance);
        }
    };
}