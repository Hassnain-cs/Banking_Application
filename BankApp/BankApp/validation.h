#pragma once
#ifndef VALIDATION_H
#define VALIDATION_H

// Input validation and error handling functions declarations

#include <stddef.h>   // for size_t

// Safe integer input
int getIntInput(const char* prompt);

// Safe integer input with min/max range checking
int getIntInRange(const char* prompt, int min, int max);

// Safe float input
float getFloatInput(const char* prompt);

// Safe float input with min/max range checking
float getFloatInRange(const char* prompt, float min, float max);

// Safe string input
void getStringInput(const char* prompt, char* output, size_t size);

#endif