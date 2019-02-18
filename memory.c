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
void map(int VA, int val) {

}

/* Store(value, virtual_address):
 * 	- perform translation of the provided virtual_address
 * 	- write the value into the physical address associated with a virtual address
 */
void store(int value, int VA) {

}

/* Load(virtual_address, value):
 *  - perform address translation of the provided virtual address
 * 	- return the byte stored in that virtual address
 */
char load(int VA, int val) {

	return 0;
}

/* * * * * * * * * * * * * * * * * * * * * * *
*	User-related methods
* * * * * * * * * * * * * * * * * * * * * * * */
int userInput(void) {

	printf("Instruction? ");

	// Ex: 0,map,0,1
	int check = scanf("%d,%5[^,],%d,%d", &processID, currInstruction, &virtualAddress, &value);

	return check;
}

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

	return 0;
}
