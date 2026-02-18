## Banking System in C

This project is a terminal-based banking application developed in the C programming language as a group project. The goal is to simulate the core features of a basic banking system while applying important programming concepts such as structures, pointers, modular programming, file handling, and input validation.

The system allows users to create accounts, log in securely, manage their money, and perform transactions while ensuring the program remains stable and does not crash even with invalid input.

---

## Goal of Project

The main goal is to build a fully functional, testable, and stable banking system in C that demonstrates strong programming logic, modular design, proper file handling, and robust error prevention. The system should behave like a simplified real-world banking application while maintaining code quality and system stability.

---

## Main Features

### Account Management
- Create account  
- Login system with password  
- Delete account  
- Account data stored using structures  

### Transactions
- Deposit money  
- Withdraw money with balance checks  
- Check account balance  
- View transaction history  

### Transfers
- Add and remove transfer contacts  
- Transfer money between accounts  
- Validation of receiver account  

### Data Persistence
- Account data saved to files  
- Transaction history saved to files  
- Data loaded automatically when program restarts  

### Stability & Validation
- Handles invalid input safely  
- Prevents negative and extreme values  
- Menu input validation  
- Protection against crashes  
- Memory safety checks  

---

## Technologies & Concepts Used

- C Programming Language  
- Structures (struct) for accounts, transactions, and contacts  
- Pointers for passing data between modules  
- Arrays and dynamic memory management  
- File handling for persistent storage  
- Modular programming with multiple .c and .h files  
- Input validation and error handling  
- Loop-driven menu system  

---
## Project Structure

The project follows a modular design where each major banking feature is separated into its own source and header file.

### Core Program
- **main.c**  
  Controls program flow, menus, and connects all modules together.

### Account Module
- **account.c**  
  Logic for creating, logging in, and managing accounts.
- **account.h**  
  Function declarations and account-related structures.

### Transaction Module
- **transaction.c**  
  Handles deposit, withdraw, balance checks, and transaction history.
- **transaction.h**  
  Transaction function declarations and structures.

### Transfer Module
- **transfer.c**  
  Manages transfer contacts and e-transfers between users.
- **transfer.h**  
  Transfer-related function declarations.

### File System Module
- **file.c**  
  Responsible for saving and loading accounts and transactions from files.
- **file.h**  
  File handling function declarations.

### Validation & Stability Module
- **validation.c**  
  Handles safe input, error checking, and crash prevention.
- **validation.h**  
  Validation function declarations.

### Data Storage
- **data/accounts.dat**  
  Stores account information.
- **data/transactions.dat**  
  Stores transaction history.


---

## Team Roles

### Helen — Account System
Responsible for user account creation, login system, password validation, and account management features.

### Inshal — Transactions
Responsible for deposit, withdraw, balance display, and transaction history.

### Hassnain — Transfers
Responsible for contact management and money transfer between users.

### Inshal, Hassnain & Kim — File Handling & Stability
Responsible for saving/loading data, input validation, error handling, and ensuring the system does not crash.

---

## Development Approach

The project is developed in structured phases:

1. Setup and system design  
2. Account system implementation  
3. Transaction system implementation  
4. Transfer system implementation  
5. File handling and stability  
6. Integration testing  
7. Final bug fixes and cleanup  

This step-by-step approach ensures all modules work together smoothly and the application remains reliable and testable.

```mermaid
timeline
    title Banking System Development Timeline

    February : Project Setup & Struct Design
             : File Structure Created
             : Input Validation Base

    Early March : Account System Complete (Inshal)

    Mid March : Transaction System Complete (Liana)

    Mid–Late March : Transfer System Complete (Hassnain)

    Late March : File Handling & Stability Complete (Kim, Inshal & Hassnain)

    Early April : Integration Testing (All)

    Before Submission : Final Bug Fixing & Code Cleanup
