#pragma once
#ifndef TRANSFER_H
#define TRANSFER_H

#include "model.h"

// Adds a new contact linked to current user
void addContact(void);

// Removes an existing contact
void removeContact(void);

// Displays saved contacts
void showContacts(void);

// Transfer money between users
void transferMoney(void);

#endif