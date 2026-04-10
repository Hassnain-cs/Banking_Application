/**
 * @file file.h
 * @brief File operations for saving and loading data
 */

#pragma once
#ifndef FILE_H
#define FILE_H

 /* Function declarations for file operations */
void loadAccountsFromFile(void);
void saveAccountsToFile(void);

void loadTransactionsFromFile(void);
void saveTransactionsToFile(void);

void loadContactsFromFile(void);
void saveContactsToFile(void);

#endif