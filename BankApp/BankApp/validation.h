#pragma once
#ifndef VALIDATION_H
#define VALIDATION_H

// Input validation and error handling functions declarations

int getIntInRange(const char* prompt, int min, int max);
void readLine(const char*prompt, char* buffer, int size);

#endif