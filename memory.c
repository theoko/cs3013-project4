#include "memory.h"

/* * * * * * * * * * * * * * * * * * * * * * *
*	Memory management methods
* * * * * * * * * * * * * * * * * * * * * * * */

/* Map(virtual_address, value):
 * 	- allocate a physical page
 * 	- create mapping in the page table between a virtual and a physical address
 * 	- if the value is set to 1, the page is writable & readable | 0 -> readable only
 * 	- calling map again with a different value should modify the existing value
 */
void map(int processID, int virtAddr, int value) {

	// Check if present in memory
	if(pArr[processID].present) {

	}

	if(hasPageTable[processID] == 0) {
		
		hasPageTable[processID] = 1;
		int VPN = virtAddr/PSIZE;
		int offset = virtAddr - VPN;
		// if there is a page table entry already, check protection bits, if they do not change, do not update. Else, update page table entry (PFN is added base on what is next available in the free_list)
		free_list[processID] = 0;
		


	} else {
	
	}	
}

/* Store(processID, virtual_address, value):
 * 	- perform translation of the provided virtual_address
 * 	- write the value into the physical address associated with a virtual address
 */
int store(unsigned char processID, unsigned char virtAddr, unsigned char value) {

	int PTEaddr = getPTEAddress(processID, virtAddr);
	int physAddr; 
	// check if it's a valid age table entry 
	// find physical adress
	if(PTEaddr >= 0){ // if the page in memory

		if (ptbrArr[processID].present == 1) {
			int tempVirtAd = virtAddr; 
			int virtPg = 0;
			while (tempVirtAd >= PSIZE) {
				if (tempVirtAd != 16)
					tempVirtAd-=PSIZE;
				
				virtPg+=1;
			}
			
         		if(memory[ptbrArr[processID].addr + (4 * virtPg) + VALID] == 0) {//~valid?	
			printf("Segmentation fault (no memory has been allocated for requested virtual address\n)");
			return 0;			
			}
						
		       	physAddr = memory[ptbrArr[processID].addr + (4 * virtPg) + PFN];
			int offset = virtAddr - (PSIZE * virtPg);
			physAddr += offset;
			
		} else {
			printf("Segmentation fault (no memory has been allocated for requested virtual address\n)");
			return 0;
		}
		
		if(memory[PTEaddr+PROTECTION] == 1) { 

			memory[physAddr] = value;
			printf("Stored value %c at virtual address %c (physical address %d\n)", value, virtAddr, physAddr);
		} else {
			printf("Virtual address %c is not writeable\n", virtAddr);
			return 0;
		}
				
	} else if (PTEaddr == -1) { // if the page is in "disk"
		int free_page = 0;
		int i;
		for (i = 0; i < PNUM; i++) {
			if (free_list[i] == 1) // if there is a free page
				free_page+=1;
		}
		
		if (ptbrArr[processID].present == 0) {
			if (free_page < 1) {
				space(1, processID);
			}

			loadProcessPageTable(processID);
			int PTEaddr = getPTEAddress(processID, virtAddr);			

			if (memory[PTEaddr+VALID] == 1) {

				free_page-=1;
				if (free_page < 1)
					space(1, processID);

				
				getPage(processID, virtAddr);
 
			}
			
			
		}

		if (memory[PTEaddr + PROTECTION] == 1) {

			if(ptbrArr[processID].present == 1){
				int tempVirtAd = virtAddr; 
				int virtPg = 0;
				while (tempVirtAd >= PSIZE) {
					if (tempVirtAd != 16)
						tempVirtAd-=PSIZE;
				
					virtPg+=1;
				}

		
				
				if(memory[ptbrArr[processID].addr + (4 * virtPg) + VALID] == 0) {//~valid?	
					printf("Segmentation fault (no memory has been allocated for requested virtual address\n)");
					return 0;			
				}
						
			       	physAddr = memory[ptbrArr[processID].addr + (4 * virtPg) + PFN];
				int offset = virtAddr - (PSIZE * virtPg);
				physAddr += offset;
			
			} else {
				printf("Segmentation fault (no memory has been allocated for requested virtual address\n)");
				return 0;
			}
	

			memory[physAddr] = value;
			printf("Stored value %c at virtual address %c (physical address %d\n)", value, virtAddr, physAddr);

		} else {
			printf("Not writeable");
		}


	} else {
		printf("Segmentation fault (no memory has been allocated for requested virtual address\n)");
		return -1;
	} 


	return 0;
	
}

/* Load(virtual_address, value):
 *  - perform address translation of the provided virtual address
 * 	- return the byte stored in that virtual address
 */
char load(unsigned char processID, unsigned char virtAddr,unsigned char value) {

	int PTEaddr = getPTEAddress(processID, virtAddr);
	
	if (PTEaddr >= 0) {

		if(memory[PTEaddr + VALID] == 1) {
			int free_page = 0;
			int i;
			for (i=0; i<PNUM; i++) {
				if(free_list[i] == 1)
					free_page = 1;
			}
			
			if (free_page) {
				getPage(processID, virtAddr);
			} else {

				space(1, processID);
				getPage(processID, virtAddr);

			}
		}
	} else if () {

	} else {
		printf("Segmentation fault (no memory has been allocated for requested virtual address\n)");
		return -1;
	}	

	return 0;
}

/* * * * * * * * * * * * * * * * * * * * * * *
*	User-related methods
* * * * * * * * * * * * * * * * * * * * * * * */
//int userInput(void) {

//	printf("Instruction? ");

	// Ex: 0,map,0,1
//	int check = scanf("%d,%5[^,],%d,%d", &processID, currInstruction, &virtualAddress, &value);
//
//	return check;
//}

/* * * * * * * * * * * * * * * * * * * * * * *
*	Main()
* * * * * * * * * * * * * * * * * * * * * * * */

int main(int argc, char** argv) {

	// userInput() will populate:
 	//	- processID
	//  - currInstruction
	//  - virtualAddress
	//  - value

//	while (userInput() != 4) {
//		printf("Invalid instruction. Please input PID,instruction,virtualAddr,value\n");
//		userInput();
//	}
//
//	if(processID < 0 || processID > 3) {
//		printf("Invalid process ID specified.\n");
//	}

	int processID; // [0, 3]
	int selection = -1; // ID to execute instruction the user selected to run
	int virtAddr; // [0,63]
	int value = -1; // [0,255]
	char input[20]; // to receive from stdin


	
	while(1){
		char * tokenInput;
		printf("Instruction? ");
		if(fgets(input, 20, stdin)!=NULL){
			
		
			while(input[0] == '\n' || input[0] == '\0' || input[1]=='\0' || input[1]=='\n'){
				printf("Instruction? ");
				if(fgets(input, 20, stdin)==NULL){
				printf("\n");
				return 0;
				}
			}

			tokenInput = strtok(input, ",");
			processID = tokenInput[0] - 48;
			tokenInput = strtok(NULL,",");

			if(tokenInput[0]=='m'){
				selection = 0;
			}	
			else if(tokenInput[0]=='s'){
				selection = 1;
			}
			else if(tokenInput[0]=='l'){
				selection = 2;
			}

			tokenInput = strtok(NULL,",");
			virtAddr = atoi(tokenInput);
			tokenInput = strtok(NULL,",");
			value = atoi(tokenInput);
			tokenInput = NULL;
			printf("\n");

			if(processID < 0 || processID > 3){
				printf("Invalid process ID specified.\n");
				selection = 3;
			}
			if(virtAddr < 0 || virtAddr > 63){
				printf("Invalid virtual address specified.\n");
				selection = 3;
			}
			if(value < 0 || value > 255){
				printf("Invalid value specified.\n");	
				selection = 3;
			}

			switch(selection){
				case 0:
					map(processID,virtAddr,value);

					break;
				case 1:
					store(processID,virtAddr,value);

					break;
				case 2:
					load(processID,virtAddr,value);
					break;
				case 3:
					break;
				default:
					printf("Invalid input specified.\n");
			}
		
			printf("\n");
		
		} else {
		    printf("\n");
		    return 0;
		}	
	}


	return 0;
}

