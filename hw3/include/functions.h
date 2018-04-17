#include <stdio.h>
#include <string.h>
#include <errno.h>
#include "debug.h"
#include "budmm.h"

//submt
void addToList(bud_free_block * address, int index);
void coalesce(bud_header* ptr, bud_header* buddy, int position);
int isBlockAllocated(bud_free_block freeBlock);
bud_free_block * findBlock(int order, int padding, int rsize);