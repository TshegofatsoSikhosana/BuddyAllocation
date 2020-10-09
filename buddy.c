
#include "buddy.h"
#include <sys/mman.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <math.h>
#include <assert.h>


struct head {
	int size;
    struct head *next;
	struct head *prev;
	bool status;
	int level;

};

struct head *new() {
	





}

struct head *buddy(struct head* block){
	




}

struct head *split(struct head* block){





}

struct head *primary(struct head* block){





}

void *hide(struct head* block){




}


struct head *magic(void *memory){




}
	

int level(int req){




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






