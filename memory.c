#include "memory.h"

/* * * * * * * * * * * * * * * * * * * * * * *
*	Memory management methods
* * * * * * * * * * * * * * * * * * * * * * * */

void space(int numFrames, int processID) {

	r();

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

		if(rr * PSIZE == ptbrArr[processID].addr && ptbrArr[processID].present == 1) { // TODO

			r();
			j++;

		}

		while(hasPageTable[rr] == 1) { // TODO

			r();
			j++;

			if(j==4) {
				break;
			}

		}

		if(removed[rr] == 0) { // TODO

			delPage(pages[rr], rr * PSIZE); // TODO
			printf("Swapped frame %d to disk at swap slot %d \n", rr, lines); // TODO
			removed[rr] = 1; // TODO
			f++;
			r();
			j++;

		} else {

			r();
			j++;

		}

		if(j == 4){
			break;
		}

	}

	// Remove page tables
	while(f < numFrames) {

		if (rr * PSIZE == ptbrArr[processID].addr && ptbrArr[processID].present == 1) { // TODO
			r();
		}

		if (removed[rr] == 0) { // TODO

			if (hasPageTable[rr] == 1) { // TODO

				removeProcessPageTable(pages[rr]); // TODO
				r();
				f++;

			} else {

				delPage(pages[rr], rr * PSIZE); // TODO
				printf("Swapped frame %d to disk at swap slot %d \n", rr, // TODO
						lines - 1);
				f++;

			}

		} else {
			r();
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

int pageToFile(unsigned char *buffer,int linenum, int base) {

	FILE* f = fopen("swap.txt", "a");//open the file

	fprintf(f,"%d ",linenum);//print out linenum

	for(int i= 0; i < PSIZE; i++){//copy in data
		fprintf(f,"%u ",buffer[base + i]);
	}

	fprintf(f,"%c ",'\n');

	fclose(f);

	return linenum;

}

int pageFromFile(unsigned char *buffer, int linenum) {

	if(linenum > lines) {
		printf("Error: line %d not found in disk \n",linenum);
		return -1;
	}

	FILE * f = fopen("swap.txt", "r");//open

	fseek(f,0,SEEK_SET);//make sure were at the top of the file

	char * token;//setup tokenize

	char line[128];//setup line

	int ln = 0;//line number interal count

	int i = 0;//iterator

	fgets(line,128,f);//grab the line

	token = strtok(line," ");//check the line number

	ln = atoi(token);

	while(ln!=linenum){//iterate until we find the line, can loop forever if not careful

		fgets(line,128,f);

		token = strtok(line," ");

		ln = atoi(token);
	}

	token = strtok(NULL," ");//get the first data number

	while(token!=NULL){//load over to the temp array

		buffer[i++] = (unsigned char) atoi(token);

		token = strtok(NULL," ");

	}

	fclose(f);

	return 1;

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

void copyToMemory(unsigned char *t, int base) {

	for(int i =0; i < PSIZE; i++){
		memory[base + i] = t[i];
	}

}

void r() {

	rr++;
	rr = rr % PNUM;

}

int loadProcessPageTable(int processID) {

	int line = ptbrArr[processID].addr;

	int f = findSpace();

	unsigned char temp[16];

	pageFromFile(temp,line);

	copyToMemory(temp, f);

	pages[f / PSIZE] = processID; // TODO

	ptbrArr[processID].addr = f;

	ptbrArr[processID].present = 1;

	hasPageTable[f / PSIZE] = 1; // TODO

	printf("Page table for process %d loaded from disk and stored at address %d\n", processID, f);

	return f;
}

void removeProcessPageTable(int processID) {

	int pt = getPTEAddress(processID, 0);

	ptbrArr[processID].addr = pageToFile(memory, lines++, pt);

	ptbrArr[processID].present = 0;

	free_list[pt / PSIZE] = 1;

	hasPageTable[pt /PSIZE] = 0;

	for(int i =0 ; i < PSIZE;i++){

		memory[pt + i] = 0;

	}

	pages[pt / PSIZE] = -1;

	printf("Page table for process %d was swapped to disk \n", processID);

}

int getPage(int processID, int virtAddr) {

	int line = memory[getPTEAddress(processID, virtAddr) + 1];

	int f = findSpace();

	unsigned char temp[16];

	pageFromFile(temp, line);

	copyToMemory(temp, f);

	pages[f / PSIZE] = processID;

	memory[getPTEAddress(processID, virtAddr) + 1] = f;

	memory[getPTEAddress(processID, virtAddr) + 3] = 2;

	printf("Page %d for process %d loaded from disk and stored at address %d\n", virtAddr / PSIZE, processID, f);

	return f;

}

void delPage(int processID, int physicalAddr) {

	int pte = getPTEAddress(processID, 0);

	int vp = 0;

	for(int i =0; i < 16; i++){

		if(memory[pte+i] == physicalAddr){

			vp = memory[pte+i-1];

		}

	}

	int fadr = pageToFile(memory, lines++, physicalAddr);

	memory[pte+(4 * vp) +PFN] = fadr;

	memory[pte+(4 * vp)+ VALID] = 1;

	for(int i =0;i < PSIZE; i++){

		memory[physicalAddr + i] = 0;

	}

	free_list[physicalAddr / PSIZE] = 1;

	pages[physicalAddr / PSIZE] = -1;

}

/* Map(processID, virtual_address, value):
 * 	- allocate a physical page
 * 	- create mapping in the page table between a virtual and a physical address
 * 	- if the value is set to 1, the page is writable & readable | 0 -> readable only
 * 	- calling map again with a different value should modify the existing value
 */
int map(unsigned char processID, unsigned char virtAddr, unsigned char value) {

	// space
	// findSpace
	// loadPageTable

	int PTEAddress = getPTEAddress(processID, virtAddr);

	if (PTEAddress == -2) {
		
		space(2, processID);

		int sfree = findSpace();

		printf("Put page table for PID %d into physical frame %d\n", processID, sfree);

		ptbrArr[processID].addr = sfree;
		ptbrArr[processID].present = 1;

		pages[sfree / PSIZE] = processID; // TODO

		hasPageTable[sfree / PSIZE] = 1; // TODO

		for(int i =0; i < PNUM; i++){
			memory[ptbrArr[processID].addr + (4 * i)] = i;
			memory[ptbrArr[processID].addr + (4 * i) + VALID] = 0;
		}

	} else if (PTEAddress == -1) {

		space(1, processID);

		int pt = loadProcessPageTable(processID);

		pages[pt / PSIZE] = processID; // TODO

		hasPageTable[pt / PSIZE] = 1; // TODO

		printf("Loaded page table for process %d at address %d\n", processID, pt);

	}

	int pte = getPTEAddress(processID, virtAddr);

	if (memory[pte + VALID] == 2){

		if(memory[pte + 2] == value) {

			printf("ERROR: Page already has value %d\n", value);

		} else{

			printf("Updated page value to %d\n", value);

			memory[pte+2] = value;
		
		}

		return 1;

	}

	int physicalAddr = findSpace();

	if(physicalAddr < 0){

		space(1, processID);

		physicalAddr = findSpace();

	}

	pages[physicalAddr / PSIZE] = processID;

	printf("Mapped virtual address %d (page %d) into physical frame %d\n", memory[pte], getVPageCount(virtAddr), physicalAddr / PSIZE);

	memory[pte + PFN] = physicalAddr;

	memory[pte + PROTECTION] = value;

	memory[pte + VALID] = 2;

	return 1;

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
					tempVirtAd -= PSIZE;
				
				virtPg+=1;

			}
			
         	if(memory[ptbrArr[processID].addr + (4 * virtPg) + VALID] == 0) {//~valid?
         		printf("Segmentation fault (no memory has been allocated for requested virtual address)\n");
         		return 0;
			}
						
		       	physAddr = memory[ptbrArr[processID].addr + (4 * virtPg) + PFN];
			int offset = virtAddr - (PSIZE * virtPg);
			physAddr += offset;
			
		} else {
			printf("Segmentation fault (no memory has been allocated for requested virtual address)\n");
			return 0;
		}
		
		if(memory[PTEaddr+PROTECTION] == 1) { 

			memory[physAddr] = value;
			printf("Stored value %u at virtual address %u (physical address %d)\n", value, virtAddr, physAddr);
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
			printf("Stored value %u at virtual address %u (physical address %d)\n", value, virtAddr, physAddr);

		} else {
			printf("Error: writes are not allowed to this page\n");
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
int load(unsigned char processID, unsigned char virtAddr, unsigned char value) {

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
	} else if (PTEaddr == -1) {













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
			printf("The value at virtual address %u (physical address %d\n) is %d\n", virtAddr, physAddr, memory[physAddr]);

		



























		
	} else {
		printf("Segmentation fault (no memory has been allocated for requested virtual address\n)");
		return -1;
	}

	return 0;
}

/* * * * * * * * * * * * * * * * * * * * * * *
*	Main()
* * * * * * * * * * * * * * * * * * * * * * * */

int main(int argc, char** argv) {

 	//	- processID
	//  - currInstruction
	//  - virtualAddress
	//  - value

	unsigned char processID; // [0, 3]
	unsigned char selection = -1; // ID to execute instruction the user selected to run
	unsigned char virtAddr; // [0,63]
	unsigned char value = -1; // [0,255]
	char input[20]; // to receive from stdin
	int k;
	rr = 0;

	FILE * temp = fopen("swap.txt","w");
	fclose(temp);

	for(k = 0; k < PNUM; k++){

		ptbrArr[k].present = -1;
		free_list[k] = 1;
		pages[k] = -1;

	}

	
	while(1) {
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
					map(processID, virtAddr, value);

					break;
				case 1:
					store(processID, virtAddr, value);

					break;
				case 2:
					load(processID, virtAddr, value);
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

