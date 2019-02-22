#ifndef MEMORY_H
#define MEMORY_H

#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <unistd.h>

// Physical memory size is 64 bytes
#define SIZE 64

// Virtual address space is 64 bytes
#define VSIZE 64

// Page size is 16 bytes
#define PSIZE 16

// Number of Pages in Memory
#define PNUM 4

struct hwStruct {

	int location; // address in memory or line in "disk"
	int inMem; // 1 if in memory, 0 if on "disk"
};

struct hwStruct hw[PNUM];

unsigned char memory[SIZE];
int hasPageTable[PNUM] // 0 if not, 1 if yes (index indicates corresponding process)
int free_list[PNUM] = {1,1,1,1}; // 1 if free, 0 if page is occupied

// Page table entry:
//   - PFN (Physical Frame Number)
//   - Protection bit (0 or 1)
//   - Valid bit (0: absent, 1: present)
//   - Present bit (0: swap off, 1: swap on)

void map(int processID, int virtAddr, int value);
int store(int processID, int virtAddr, int value);
char load(int processID, int virtAddr, int value); // val not used in this

//int userInput(void);



#endif
