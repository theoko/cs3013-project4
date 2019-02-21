#ifndef MEMORY_H
#define MEMORY_H

#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include <string.h>

// Physical memory size is 64 bytes
#define SIZE 64

// Virtual address space is 64 bytes
#define VSIZE 64

// Page size is 16 bytes
#define PSIZE 16

unsigned char memory[SIZE];

// Page table entry:
//   - PFN (Physical Frame Number)
//   - Protection bit (0 or 1)
//   - Valid bit (0: absent, 1: present)
//   - Present bit (0: swap off, 1: swap on)


void map(int processID, int virtAddr, int value);
void store(int processID, int virtAddr, int value);
char load(int processID, int virtAddr, int value); // val not used in this

//int userInput(void);



#endif
