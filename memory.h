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

// Number of Pages in Memory
#define PNUM 4


unsigned char memory[SIZE];
int hasPageTable[PNUM] // 0 if not, 1 if yes (index indicates corresponding process)
int free_list[PNUM] = {1,1,1,1}; // 1 if free, 0 if page is occupied
int hasPageTable[4]; // 0 if not, 1 if yes (index indicates corresponding process)
int free_list[4] = {1,1,1,1}; // 1 if free, 0 if page is occupied


// Page table entry:
//   - PFN (Physical Frame Number)
//   - Protection bit (0 or 1)
//   - Valid bit (0: absent, 1: present)
//   - Present bit (0: swap off, 1: swap on)
typedef struct pnode {
	int addr;
	int present;
} p_node;


void map(int processID, int virtAddr, int value);
int store(int processID, int virtAddr, int value);
char load(int processID, int virtAddr, int value); // val not used in this

//int userInput(void);

p_node pArr[4];


void map(unsigned char processID, unsigned char virtAddr, unsigned char value);
void store(unsigned char processID, unsigned char virtAddr, unsigned char value);
char load(unsigned char processID, unsigned char virtAddr, unsigned char value); // val not used in this

//int userInput(void);

#endif
