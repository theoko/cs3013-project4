#ifndef MEMORY_H
#define MEMORY_H

#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <unistd.h>

#define SIZE 64

unsigned char memory[SIZE];

void map(int VA, int val);
void store(int value, int VA);
char load(int VA, int val); // val not used in this

void userInput(void);

int processID; // [0, 3]
char *currInstruction; // Store desired memory operation
int virtualAddress; // [0, 63]
int value; // value [0, 255]

#endif
