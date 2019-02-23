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


#define VPN 0

#define PFN 1

#define PROTECTION 2

#define VALID 3

unsigned char memory[SIZE];


int hasPageTable[PNUM]; // 0 if not, 1 if yes (index indicates corresponding process)

int free_list[PNUM] = {1,1,1,1}; // 1 if free, 0 if page is occupied

int pages[PNUM];

int rr;
int lines;

// Page table entry:
//   - PFN (Physical Frame Number)
//   - Protection bit (0 or 1)
//   - Valid bit (0: absent, 1: present)
//   - Present bit (0: swap off, 1: swap on)
typedef struct PTBR_str {
	int addr;
	int present;
} ptbr;


// Helper methods
int getVPageCount(int virtAddr);
int getPTEAddress(int processID, int virtAddr);
void copyToMemory(unsigned char *procID, int base);

// Round-robin
void r();

// Space allocation
void space(int numFrames, int processID);
int findSpace();

// Swapping
int pageToFile(unsigned char *buffer,int linenum, int base);
int pageFromFile(unsigned char *buffer, int linenum);

// Process page table
int loadProcessPageTable(int processID);
void removeProcessPageTable(int processID);

// Pages
int getPage(int processID, int virtAddr);
int delPage(int processID, int physicalAddr);

// Map, Load and Store
int map(unsigned char processID, unsigned char virtAddr, unsigned char value);
int store(unsigned char processID, unsigned char virtAddr, unsigned char value);
int load(unsigned char processID, unsigned char virtAddr, unsigned char value); // val not used in this


ptbr ptbrArr[4];

#endif
