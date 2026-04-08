#include "pch.h"
#include "CppUnitTest.h"

extern "C" {
#include "../BankApp/account.h"
#include "../BankApp/transaction.h"
#include "../BankApp/transfer.h"
#include "../BankApp/file.h"
    void withdrawFromAccount(Account* acc, double amount);
}

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace BankingTests
{
    TEST_CLASS(WithdrawTests)
    {
    public:

        // VALID WITHDRAW
        TEST_METHOD(Withdraw_Valid)
        {
            Account acc;
            acc.balance = 200.0;

            withdrawFromAccount(&acc, 50.0);

            Assert::AreEqual(150.0, acc.balance);
        }

        // WITHDRAW FULL BALANCE
        TEST_METHOD(Withdraw_FullBalance)
        {
            Account acc;
            acc.balance = 100.0;

            withdrawFromAccount(&acc, 100.0);

            Assert::AreEqual(0.0, acc.balance);
        }

        // INSUFFICIENT FUNDS (should NOT change)
        TEST_METHOD(Withdraw_InsufficientFunds)
        {
            Account acc;
            acc.balance = 100.0;

            withdrawFromAccount(&acc, 150.0);

            Assert::AreEqual(100.0, acc.balance);
        }

        // ZERO AMOUNT (no change)
        TEST_METHOD(Withdraw_Zero)
        {
            Account acc;
            acc.balance = 100.0;

            withdrawFromAccount(&acc, 0.0);

            Assert::AreEqual(100.0, acc.balance);
        }

        // NEGATIVE AMOUNT (no change)
        TEST_METHOD(Withdraw_Negative)
        {
            Account acc;
            acc.balance = 100.0;

            withdrawFromAccount(&acc, -50.0);

            Assert::AreEqual(100.0, acc.balance);
        }

        // NULL ACCOUNT (no crash)
        TEST_METHOD(Withdraw_NullAccount)
        {
            Account* acc = nullptr;

            withdrawFromAccount(acc, 50.0);

            Assert::IsTrue(true);
        }

        // MULTIPLE WITHDRAWS
        TEST_METHOD(Withdraw_MultipleTransactions)
        {
            Account acc;
            acc.balance = 300.0;

            withdrawFromAccount(&acc, 50.0);
            withdrawFromAccount(&acc, 50.0);
            withdrawFromAccount(&acc, 100.0);

            Assert::AreEqual(100.0, acc.balance);
        }
    };
}