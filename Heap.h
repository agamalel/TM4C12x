// Heap memory manager
// allocates memory and free it
// By Mo'meN Ali

#include <stdint.h>

// Initialize the Heap
// creates a linear linked lists
// first element of each block contains a pointer to the next free block
void Heap_Init(void);

// allocates a fixed-block-size block
// returns a pointer to the allocated block 
// returns NULL if there's not
void *Heap_Alloc(void);

// frees a pre-allocated block
// takes a pointer to a pre-allocated block as argument
// does not check if the the block is previously allocated or free
// the freed block gets back into the begining of the heap
void Heap_Free(int32_t *BlockPt);
