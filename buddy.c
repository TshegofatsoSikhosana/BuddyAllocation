
#include "buddy.h"
#include <sys/mman.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <math.h>
#include <assert.h>


#define MIN 5
#define MAX 12

struct head {
  struct head *next;
	struct head *prev;
	bool status;
	int level;
};

struct head *new() {

  size_t size = (size_t)pow(2,13);
	struct head * region = mmap(
	(void*) size,   // Map from the start of the 2^12th page
	size,                         // for one page length
	PROT_READ|PROT_WRITE|PROT_EXEC,
	MAP_ANON|MAP_PRIVATE,             // to a private block of hardware memory
	0,
	0
	);

	if (region == MAP_FAILED) {
		perror("Could not mmap");
		return NULL;
	}

		region->status = true;
		region->level = 7;
		region->next = NULL;
		region->prev = NULL;


	return region;
}

struct head *buddy(struct head* block){


	//top most block
	if(block->level == 6)
	{
		return NULL;
	}

	//is the beginning
	if(block->prev == NULL)
	{
		if(block->next != NULL)
		{
			return block->next;
		}
		else
		{
			perror(" \n Couldn't find buddy \n");
		}
	}
	else
	{
		return block->prev;
	}

}

struct head *split(struct head* block){

	if(block->level != 0)
	{
		block->level = block->level -1;
		block->next = block + (int)(pow(2,block->level _ MIN)/2);

		//point to next and point back

		return block;
	}
	else
	{
		perror("You can't split");
		return block;
	}

}

struct head *merge(struct head* h1,struct head* h2)
{
	struct head* merged_block = primary(h1);

	if(merged_block == h1)
	{
		merged_block->level  = h1->level +1;

		//merger happens here

	}
	else
	{
		merged_block = primary(h2);
	}



	return merged_block;
}

struct head *primary(struct head* block){

	if(block->prev == NULL)
	{
		return block;
	}
	else
	{
		return block->prev;
	}

}

void *hide(struct head* block){

}

struct head *magic(void *memory){

}

int level(int req){

	return (int)round(log2(req)) - MIN;
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

// Function to calculate x raised to the power y */
int power(int x,int y)
{
	int ans =1;

  for(int i = 1;i <= y;i++)
	{
			ans = ans*2;
	}

	return ans;
}
