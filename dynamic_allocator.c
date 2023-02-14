/*
 * dyn_block_management.c
 *
 *  Created on: Sep 21, 2022
 *      Author: HP
 */
#include <inc/assert.h>
#include <inc/string.h>
#include "../inc/dynamic_allocator.h"
 //==================================================================================//
 //============================== GIVEN FUNCTIONS ===================================//
 //==================================================================================//

 //===========================
 // PRINT MEM BLOCK LISTS:
 //===========================

void print_mem_block_lists() {
	cprintf("\n=========================================\n");
	struct MemBlock* blk;
	struct MemBlock* lastBlk = NULL;
	cprintf("\nFreeMemBlocksList:\n");
	uint8 sorted = 1;
	LIST_FOREACH(blk, &FreeMemBlocksList)
	{
		if (lastBlk && blk->sva < lastBlk->sva + lastBlk->size)
			sorted = 0;
		cprintf("[%x, %x)-->", blk->sva, blk->sva + blk->size);
		lastBlk = blk;
	}
	if (!sorted)
		cprintf("\nFreeMemBlocksList is NOT SORTED!!\n");

	lastBlk = NULL;
	cprintf("\nAllocMemBlocksList:\n");
	sorted = 1;
	LIST_FOREACH(blk, &AllocMemBlocksList)
	{
		if (lastBlk && blk->sva < lastBlk->sva + lastBlk->size)
			sorted = 0;
		cprintf("[%x, %x)-->", blk->sva, blk->sva + blk->size);
		lastBlk = blk;
	}
	if (!sorted)
		cprintf("\nAllocMemBlocksList is NOT SORTED!!\n");
	cprintf("\n=========================================\n");

}

//********************************************************************************//
//********************************************************************************//

//==================================================================================//
//============================ REQUIRED FUNCTIONS ==================================//
//==================================================================================//


//===============================
//NOTE:
// Each memory block has a start virtual address "sva" and a "size" 
//===============================

//===============================
// [1] INITIALIZE AVAILABLE LIST:
//===============================
void initialize_MemBlocksList(uint32 numOfBlocks) {
	//TODO: [PROJECT MS1] [DYNAMIC ALLOCATOR] initialize_MemBlocksList

	LIST_INIT(&AvailableMemBlocksList);
	for (int i = 0; i < numOfBlocks; i++) {
		LIST_INSERT_HEAD(&AvailableMemBlocksList, &(MemBlockNodes[i]));
	}
}

//===============================
// [2] FIND BLOCK:
//===============================
struct MemBlock* find_block(struct MemBlock_List* blockList, uint32 va) {
	//TODO: [PROJECT MS1] [DYNAMIC ALLOCATOR] find_block

	struct MemBlock* ptr;
	bool found_flag = 0;
	LIST_FOREACH(ptr, blockList)
	{
		if (ptr->sva == va) {
			found_flag = 1;
			break;
		}

	}
	if (found_flag == 0) {
		return NULL;
	}
	return ptr;
}



//=========================================
// [3] ALLOCATE BLOCK BY FIRST FIT:
//=========================================
struct MemBlock* alloc_block_FF(uint32 size) {
	//TODO: [PROJECT MS1] [DYNAMIC ALLOCATOR] alloc_block_FF
	struct MemBlock* element;
	bool found = 0;
	LIST_FOREACH(element, &FreeMemBlocksList)
	{

		if (element->size == size) {

			LIST_REMOVE(&FreeMemBlocksList, element);
			return element;
		}

		else if (element->size > size) {

			struct MemBlock* returnedblock = NULL;
			returnedblock = AvailableMemBlocksList.lh_first;
			LIST_REMOVE(&AvailableMemBlocksList, returnedblock);
			returnedblock->sva = element->sva;
			returnedblock->size = size;

			element->size = element->size - size;
			element->sva = element->sva + size;
			return returnedblock;
		}

	}

	return NULL; // in case no block list fits the input size

}

//=========================================
// [4] ALLOCATE BLOCK BY BEST FIT:
//=========================================
struct MemBlock* alloc_block_BF(uint32 size) {
	//TODO: [PROJECT MS1] [DYNAMIC ALLOCATOR] alloc_block_BF
	int sizee = 2147483647;
	int svaa = LIST_FIRST(&FreeMemBlocksList)->sva;
	struct MemBlock* element1;
	struct MemBlock* element2;
	LIST_FOREACH(element1, &FreeMemBlocksList)
	{

		if (element1->size == size) {
			LIST_REMOVE(&FreeMemBlocksList, element1);
			return element1;
		}
		else if (element1->size > size) {
			if (element1->size < sizee) {
				sizee = element1->size;
				svaa = element1->sva;
			}
		}

	}
	LIST_FOREACH(element2, &FreeMemBlocksList)
	{
		if (element2->size == sizee) {
			struct MemBlock* returnedblock = NULL;
			returnedblock = AvailableMemBlocksList.lh_first;
			LIST_REMOVE(&AvailableMemBlocksList, returnedblock);
			returnedblock->sva = element2->sva;
			returnedblock->size = size;
			element2->size = element2->size - size;
			element2->sva = element2->sva + size;
			return returnedblock;

		}
	}
	return NULL;

}


}

