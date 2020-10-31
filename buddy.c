
#include "buddy.h"
#include <sys/mman.h>
#include <stddef.h>
#include <stdio.h>
#include <math.h>
#include <assert.h>
#include <stdbool.h>

#define MINIMUM 5
#define MAXIMUM 12

struct head {

	int level;
	struct head *next;
	struct head *prev;
	bool status;

};

struct head *new() {


	size_t 	pagesize = (size_t)pow(2, MAXIMUM);
	struct head* map = mmap(
	(void *) pagesize,
	pagesize,
	PROT_READ|PROT_WRITE|PROT_EXEC,
	MAP_ANON|MAP_PRIVATE,
	0,
	0);

	if(map == MAP_FAILED){
		fprintf(stderr, "malloc failed\n");
		return NULL;
	}

	map->status = true;
	map->level = 7;
	map->next = NULL;
	map->prev = NULL;

	return map;

}

struct head *buddy(struct head* block){

	if(block->level == 7){
		fprintf(stderr, "Does not have buddy\n");
		return NULL;
	}
	else if(block->next == NULL){
		return block->prev;
	}
	else{
		return block->next;
	}



}

struct head *split(struct head* block){

	if(block->level == 0){
		fprintf(stderr, "Cannot split\n");
		return block;
	}

	//struct head *temp = block->next;

	block->level = block->level - 1;
	block->next = block + (int)(pow(2, block->level+MINIMUM)/2);

	// struct head *next = block->next;
	// next->next = temp;
	// next->prev = block;

	return block;


}

struct head *primary(struct head* block){

	if(block->prev != NULL){
		return block->prev;
	}else{
		return block;
	}

}

struct head *merge(struct head* blockA, struct head* blockB){

	if(primary(blockA) == blockA){
		blockA->next = blockB->next;
		blockA->level = blockA->level + 1;
		return blockA;
	}
	else{
		blockB->next = blockA->next;
		blockB->level = blockB->level + 1;
		return blockB;
	}

}

void *hide(struct head* block){

	return (block + sizeof(struct head));

}


struct head *magic(void *memory){

	return (struct head*)(memory - sizeof(struct head));

}


int level(int req){

	if(req > 4096){
		fprintf(stderr, "Request too big \n");
		return 1;
	}
	else{
		double x = round(log2(req));
		int level = (int) x;
		return level - MINIMUM;
	}
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
