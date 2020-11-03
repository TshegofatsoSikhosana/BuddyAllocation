#define MIN 5
#define LEVELS 8
#define PAGE 4096
#include "buddy.h"
#include <sys/mman.h>
#include <stddef.h>
#include <stdio.h>
#include <math.h>
#include <assert.h>

enum flag{Free,Taken};

struct head {
	enum flag status;
	short int level;
	struct head *next;
	struct head *prev;
};

struct head *new() {
	struct head *new = (struct head *) mmap(NULL,PAGE,PROT_READ | PROT_WRITE,MAP_PRIVATE | MAP_ANONYMOUS,-1,0);

	if(new == MAP_FAILED){
		return NULL;
	}
	assert (((long int) new & 0xfff) ==0); //checking that the 12 last bits should be zero
	new->status = Free;
	new->level = LEVELS -1;
	return new;
}

struct head *buddy(struct head* block){
	int index = block->level;
	long int mask = 0x1 << (index + MIN);
	return (struct head*)((long int) block ^ mask);
}

struct head *split(struct head* block){
	int index = block->level-1;
	int mask = 0x1 << (index + MIN);
	return (struct head*)((long int)block | mask);
}

struct head *primary(struct head* block){
	int index = block->level;
	long int mask = 0xffffffffffffffff << (1 + index + MIN);
	return (struct head*)((long int)block & mask);
}

void *hide(struct head* block){
	return (void*)(block +1);
}

struct head *magic(void *memory){
	return ((struct head*)memory-1);
}

int level(int req){
	int total = req + sizeof(struct head);
	int i=0;
	int size =1 << MIN;
	while(total > size){
		size <<=1;
		i += 1;
	}
	return i;

}


/** Mark a data block as free and merge free buddy blocks **/
void bfree(void *ptr)
{
    // TODO
    // check if pointer is valid
    if (!valid_address(ptr))
    {
        return;
    }
    // get block
    block *b = get_block(ptr);

    // check if block was found
    if (!b)
    {
        return;
    }

    // set free to true
    b->free = true;

    // check if buddy is free
    while (b->buddy && b->buddy->free && b->size == b->buddy->size)
    {
        // merge blocks
        b = merge_block(b);
    }
    ptr = b->data;
    //b->data = NULL;
}

/** Change the memory allocation of the data to have at least the requested size **/
void *balloc(void *ptr, size_t size)
{
	// TODO
	// check size
	if (size <= 0)
	{
			return NULL;
	}
	// create block and find nearest block size
	block *b;
	int s = find_nearest_block_size(size + sizeof(block));

	// check if head is null
	if (!head)
	{
			// create head block
			head = (block *)sbrk(MAX_SIZE);
			head->buddy = NULL;
			head->free = true;
			head->next = NULL;
			head->size = MAX_SIZE;
			for (int i = 0; i < MAX_EXP; i++)
			{
					head->merge_buddy[i] = NULL;
			}
			head->data = (void *)head + sizeof(block);
	}

	// find block that can fit the size that's required
	b = find_block(s);

	// check if block was found
	if (!b)
	{
			return NULL;
	}

	// reduce block to appropriate size
	while (((int)b->size) > s)
	{
			split_block(b);
	}

	b->free = false;
	b->data = (void *)b + sizeof(block);
	return b->data;
}


void dispblocklevel(struct head* block){
	printf("block level = %d\n",block->level);
}
void dispblockstatus(struct head* block){
	printf("block status = %d\n",block->status);
}

void blockinfo(struct head* block){
	printf("===================================================================\n");
	dispblockstatus(block);
	dispblocklevel(block);
	printf("start of block in memory: %p\n", block);
	printf("size of block in memory: %ld in bytes\n",sizeof(struct head));
	printf("===================================================================\n");
}
