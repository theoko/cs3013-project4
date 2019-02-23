In order to run:

			make
			./mem


Can do: make clean (which also removes the 'swap.txt' file)

For testing: A txt file with instructions can be piped in order to test the program


In our program, a page table entry consists of a VPN (0th byte), a PFN (1st byte), a PROTECTION byte (2nd byte) and a VALID byte (3rd byte).
A page table consists of four (4) page table entries (16 bytes total which is equal to the page size).


For our eviction strategy, we implement evicting by applying a variation of a round robin in order to remove pages when memory is full. We have a global int counter which increments and uses mod 4 (number of pages) whenever something is removed. Thus, we iterate and remove pages successively (if page to be removed is the page table of the current pid, round robin global will be incremented without removing the page...). I  

Test 1 and Test 2 where provided in the document (project description) and Test 3 tests "excessive" swapping.
