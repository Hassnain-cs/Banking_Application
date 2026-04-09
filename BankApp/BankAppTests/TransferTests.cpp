#include "pch.h"
#include "CppUnitTest.h"

extern "C" {
#include "../BankApp/account.h"
#include "../BankApp/transfer.h"
    void transferBetweenAccounts(Account* sender, Account* receiver, double amount);
}

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace BankingTests
{
    TEST_CLASS(TransferTests)
    {
    public:

        // VALID TRANSFER
        TEST_METHOD(Transfer_Valid)
        {
            Account sender, receiver;
            sender.balance = 200.0;
            receiver.balance = 100.0;

            transferBetweenAccounts(&sender, &receiver, 50.0);

            Assert::AreEqual(150.0, sender.balance);
            Assert::AreEqual(150.0, receiver.balance);
        }

        // INSUFFICIENT FUNDS (no change)
        TEST_METHOD(Transfer_InsufficientFunds)
        {
            Account sender, receiver;
            sender.balance = 100.0;
            receiver.balance = 100.0;

            transferBetweenAccounts(&sender, &receiver, 150.0);

            Assert::AreEqual(100.0, sender.balance);
            Assert::AreEqual(100.0, receiver.balance);
        }

        // ZERO AMOUNT (no change)
        TEST_METHOD(Transfer_Zero)
        {
            Account sender, receiver;
            sender.balance = 100.0;
            receiver.balance = 100.0;

            transferBetweenAccounts(&sender, &receiver, 0.0);

            Assert::AreEqual(100.0, sender.balance);
            Assert::AreEqual(100.0, receiver.balance);
        }

        // NEGATIVE AMOUNT (no change)
        TEST_METHOD(Transfer_Negative)
        {
            Account sender, receiver;
            sender.balance = 100.0;
            receiver.balance = 100.0;

            transferBetweenAccounts(&sender, &receiver, -50.0);

            Assert::AreEqual(100.0, sender.balance);
            Assert::AreEqual(100.0, receiver.balance);
        }

        // NULL SENDER
        TEST_METHOD(Transfer_NullSender)
        {
            Account receiver;
            receiver.balance = 100.0;

            transferBetweenAccounts(NULL, &receiver, 50.0);

            Assert::AreEqual(100.0, receiver.balance);
        }

        // NULL RECEIVER
        TEST_METHOD(Transfer_NullReceiver)
        {
            Account sender;
            sender.balance = 100.0;

            transferBetweenAccounts(&sender, NULL, 50.0);

            Assert::AreEqual(100.0, sender.balance);
        }

        // BOTH NULL (no crash)
        TEST_METHOD(Transfer_BothNull)
        {
            transferBetweenAccounts(NULL, NULL, 50.0);

            Assert::IsTrue(true);
        }

        // FULL BALANCE TRANSFER
        TEST_METHOD(Transfer_FullBalance)
        {
            Account sender, receiver;
            sender.balance = 100.0;
            receiver.balance = 50.0;

            transferBetweenAccounts(&sender, &receiver, 100.0);

            Assert::AreEqual(0.0, sender.balance);
            Assert::AreEqual(150.0, receiver.balance);
        }

        // MULTIPLE TRANSFERS
        TEST_METHOD(Transfer_MultipleTransactions)
        {
            Account sender, receiver;
            sender.balance = 500.0;
            receiver.balance = 100.0;

            transferBetweenAccounts(&sender, &receiver, 100.0);
            transferBetweenAccounts(&sender, &receiver, 50.0);
            transferBetweenAccounts(&sender, &receiver, 150.0);

            Assert::AreEqual(200.0, sender.balance);
            Assert::AreEqual(400.0, receiver.balance);
        }
    };
}