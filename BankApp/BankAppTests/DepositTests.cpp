#include "pch.h"
#include "CppUnitTest.h"

extern "C" {
#include "../BankApp/account.h"
#include "../BankApp/transaction.h"
}

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace BankingTests
{
    TEST_CLASS(DepositTests)
    {
    public:

        TEST_METHOD(Deposit_Valid)
        {
            Account acc = { 0 };
            acc.balance = 100;

            depositToAccount(&acc, 50);

            Assert::AreEqual(150.0, acc.balance);
        }
    };
}