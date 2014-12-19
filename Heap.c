// Heap memory manager
// allocates memory and free it
// By Mo'meN Ali

#include <stdint.h>
#include <stdlib.h>
#include "Heap.h"

#define BLOCKSIZE	80
#define BLOCKNUM	5
int32_t Heap[BLOCKSIZE * BLOCKNUM];
int32_t *HeadPt;  // pointer to the head of the heap

// Initialize the Heap
// creates a linear linked lists
// first element of each block contains a pointer to the next free block
void Heap_Init(void)
{
	int32_t *BlockPt;
 
	HeadPt= Heap;  // save the heap head address
	for(BlockPt= Heap;		// start from the first block of the heap
			BlockPt != &Heap[BLOCKSIZE * (BLOCKNUM - 1)];		// leave the last block because it should point to NULL 
			BlockPt+= BLOCKSIZE)				// Next Block
	{
		*BlockPt= (int32_t)(BlockPt + BLOCKSIZE);		// cast the address of the next block as a value so we can save it into an array element (variable)
	}
	*BlockPt= NULL; 		  // no more blocks, so last allocated block points to NULL
}

// allocates a fixed-block-size block
// returns a pointer to the allocated block 
// returns NULL if there's not
void *Heap_Alloc(void)
{
	int32_t *BlockPt;
	
	BlockPt= HeadPt;		// take the first block
	if(BlockPt != NULL) // if it's a valid block
	{ 															// take the address of the next block
		HeadPt= (int32_t *)*BlockPt;  // make sure to cast it as an address because we saved it as a normal number
	}

	return (BlockPt);			// return pointer-to the allocated block or NULL
}

// frees a pre-allocated block
// takes a pointer to a pre-allocated block as an argument
// does not check if the the block is previously allocated or free
// the freed block gets back into the begining of the heap
void Heap_Free(int32_t *BlockPt)
{
	int32_t *tempPt;
	
	tempPt= HeadPt;   // save the current head of the empty block-list
	HeadPt= BlockPt;  // save the block to be freed
	*HeadPt= (int32_t)tempPt;  // link it to the begining of the list
	BlockPt= NULL;
}
