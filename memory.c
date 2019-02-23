#include "memory.h"

/* * * * * * * * * * * * * * * * * * * * * * *
*	Memory management methods
* * * * * * * * * * * * * * * * * * * * * * * */

void space(int numFrames, int processID) {

	robin();

	int f = 0;

	for(int i= 0; i < PNUM; i++) {
		if(free_list[i]==1){
			f++;
		}
	}

	int removed[4];

	for(int i =0; i < 4; i++){
		removed[i] = 0;
	}

	while(f < numFrames){

		int j = 0;
		if(roundrobin * PSIZE == ptbrArr[processID].addr && ptbrArr[processID].present == 1) { // TODO

			robin();
			j++;

		}

		while(ispagetable[roundrobin] == 1) { // TODO

			robin();
			j++;
			if(j==4){
				break;
			}

		}

		if(removed[roundrobin] == 0) { // TODO

			removePage(pages[roundrobin], roundrobin * PAGEMEM); // TODO
			printf("Swapped frame %d to disk line %d \n", roundrobin, linecount); // TODO
			removed[roundrobin] = 1; // TODO
			f++;
			robin();
			j++;

		} else {

			robin();
			j++;

		}

		if(j==4){
			break;
		}
	}

	// Remove page tables
	while(f < numOfFrames) {

		if (roundrobin * PAGEMEM == ptbrArr[processID].addr && ptbrArr[processID].present == 1) { // TODO
			robin();
		}

		if (removed[roundrobin] == 0) { // TODO

			if (ispagetable[roundrobin] == 1) { // TODO
				removePageTable(pages[roundrobin]); // TODO
				robin();
				f++;
			} else {
				removePage(pages[roundrobin], roundrobin * PSIZE); // TODO
				printf("Swapped frame %d to disk line %d \n", roundrobin, // TODO
						linecount - 1);
				f++;
			}

		} else {
			robin();
		}
	}

}

int findSpace() {
	for(int i =0; i < PNUM; i++){
		if(free_list[i]==1){
			free_list[i] = 0;
			return i * PSIZE;
		}
	}

	// No space left
	return -1;
}

int getVPageCount(int virtAddr) {
	int tmp = virtAddr;

	int countPages = 0;

	while(tmp > PSIZE) {
		tmp -= PSIZE;
		countPages++;
	}

	if(tmp == PSIZE)
		countPages++;

	return countPages;
}

int getPTEAddress(int processID, int virtAddr) {
	// Check if present in memory
	if(ptbrArr[processID].present == 1) {

		int vPages = getVPageCount(virtAddr);

		return (4 * vPages) + ptbrArr[processID].addr;

	} else if (ptbrArr[processID].present == 0) {
		return -1;
	} else {
		return -2;
	}
}

/* Map(processID, virtual_address, value):
 * 	- allocate a physical page
 * 	- create mapping in the page table between a virtual and a physical address
 * 	- if the value is set to 1, the page is writable & readable | 0 -> readable only
 * 	- calling map again with a different value should modify the existing value
 */
int map(int processID, int virtAddr, int value) {

	// freespace
	// findfree
	// loadPageTable

	int PTEAddress = getPTEAddress(processID, virtAddr);

	if (PTEAddress == -2) {
		
		space(2, processID);

		int sfree = findSpace();

		printf("Page table for process %d created at address %d\n", processID, sfree);

		ptbrArr[processID].addr = sfree;
		ptbrArr[processID].present = 1;

		pages[free / PSIZE] = processID; // TODO

		ispagetable[free / PSIZE] = 1; // TODO

		for(int i =0; i < PNUM; i++){
			memory[ptbrArr[processID].addr + (4 * i)] = i;
			memory[ptbrArr[processID].addr + (4 * i) + VALID] = 0;
		}

	} else if (PTEAddress == -1) {

		freespace(1, processID);

		int pt = loadPageTable(processID);

		pages[pt / PSIZE] = processID; // TODO

		ispagetable[pt / PSIZE] = 1; // TODO

		printf("Loaded page table for process %d at address %d\n", processID, pt);

	}

	int pte = getPTEAddress(processID, virtAddr);

	if (memory[pte + VALID] == 2){

		if(memory[pte + 2] == value) {

			printf("ERROR: Page already has value %d \n", value);

		} else{

			printf("Altered Page value\n");

			memory[pte+2] = value;
		
		}

		return 1;

	}

	int ppage = findfree();

	if(ppage < 0){

		freespace(1, processID);

		ppage = findfree();

	}

	pages[ppage/16] = processID;

	printf("Physical page for VPN %d allocated at: %d\n", memory[pte],ppage);

	memory[pte + PFN] = ppage;

	memory[pte + PROTECTION] = value;

	memory[pte + VALID] = 2;

	return 1;

}

/* Store(processID, virtual_address, value):
 * 	- perform translation of the provided virtual_address
 * 	- write the value into the physical address associated with a virtual address
 */
int store(int processID, int virtAddr, int value) {
	printf("store\n");
	int physAddr; 
	// check if it's a valid age table entry 
	// find physical adress
	if(page table entry is valid) {
		// convert
		if (ptbrArr[processID].present == 1) {
			int tempVirtAd = virtAddr; 
			int virtPg = 0;
			while (tempVirtAd >= PSIZE) {
				if (tempVirtAd != 16)
					tempVirtAd-=PSIZE;
				
				virtPg+=1;
			}
			
			// QUESTION HERE-- do rest of if statement  !!!!!!!!!
		} else {
			printf("Segmentation fault (no memory has been allocated for requested virtual address\n)");
			physAddr = -1;	
		}
		
		if(page can be edited, store value) {

			memory[physAddr] = value;
			printf("Stored value %d at virtual address %d (physical address %d\n)", value, virtAddr, physAddr);
		} else {
			printf("Virtual address %d is not writable\n");
			return 0;
		}
				
	} else if (page tble entry result is -1 - if page is in disk) {
		int free_page = 0;
		int i;
		for (i = 0; i < PNUM; i++) {
			if (free_list[i] == 1) // if there is a free page
				free_page+=1;
		}
		
		if ()

	} else {
		printf("Segmentation fault (no memory has been allocated for requested virtual address\n)");
		return -1;
	} 
	//--------------------------------------

	return 0;
	
}

/* Load(virtual_address, value):
 *  - perform address translation of the provided virtual address
 * 	- return the byte stored in that virtual address
 */
int load(int processID, int virtAddr,int value) {
	printf("load\n");	
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

 	//	- processID
	//  - currInstruction
	//  - virtualAddress
	//  - value

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

