/*
 * All functions you make for the assignment must be implemented in this file.
 * Do not submit your assignment with a main function in this file.
 * If you submit with a main function in this file, you will get a zero.
 */
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include "debug.h"
#include "budmm.h"
#include "functions.h"

/*
 * You should store the heads of your free lists in these variables.
 * Doing so will make it accessible via the extern statement in budmm.h
 * which will allow you to pass the address to sf_snapshot in a different file.
 */
// try again
extern bud_free_block free_list_heads[NUM_FREE_LIST];
void addToList(bud_free_block * address, int index){
bud_free_block *sentinel = &free_list_heads[index];
                 bud_free_block *next = sentinel->next;
                 address->next = next;
                 address->prev = sentinel;
                 sentinel->next = address;
                 (*next).prev = address;
}

void  coalesce(bud_header* ptr, bud_header* buddy, int position){
    if((buddy -> allocated != 0)||(buddy -> order != ptr -> order)){
        ptr -> allocated = 0;
        int index = ptr-> order - ORDER_MIN;
        bud_free_block * fb = (bud_free_block *)ptr;
        fb -> header = *ptr;
        fb -> next = NULL;
        fb -> prev = NULL;
        addToList(fb,index);
    }
    else{
        if(position == 0){
        int order = ptr -> order ;
        order = order + 1;
        ptr -> order = order;
        ptr -> allocated = 0;
        int size = ORDER_TO_BLOCK_SIZE(order);
        bud_header* buddyAddress = (bud_header *)((uintptr_t)ptr ^ (uintptr_t)size);
        int position = 0;
        if((uintptr_t) buddyAddress < (uintptr_t)ptr){
            position = 1;
        }
        bud_free_block *remove = (bud_free_block *) buddy;
        (remove -> next)->prev = remove-> prev;
        (remove -> prev)->next = remove -> next;
        remove -> next = NULL;
        remove -> prev = NULL;
        coalesce(ptr,buddyAddress, position);
    }
    else{
        int order = buddy -> order ;
        order = order + 1;
        buddy -> order = order;
        buddy -> allocated = 0;
        int size = ORDER_TO_BLOCK_SIZE(order);
        bud_header* ptr = (bud_header *)((uintptr_t)buddy ^ (uintptr_t)size);
       int position = 0;
       if((uintptr_t) ptr < (uintptr_t)buddy){
        position = 1;
       }
        bud_free_block *remove = (bud_free_block *) buddy;
        (remove -> next)->prev = remove -> prev;
        (remove -> prev)->next = remove -> next;
        remove -> next = NULL;
        remove -> prev = NULL;
        coalesce(buddy,ptr,position);

    }
    }

}

int isBlockAllocated(bud_free_block freeBlock){
return 0;
}

bud_free_block * findBlock(int order, int padding, int rsize){
int index = order - ORDER_MIN;
int oldIndex =index;
// submit again
/* if we want to allocate the max block there is no splitting */
if(index == ((ORDER_MAX - ORDER_MIN)-1)){
    if(bud_sbrk()== (void*)-1){
                return NULL;
            }
    bud_free_block * address = bud_heap_start();
    bud_header bh;
    bh.allocated = 1;
    bh.order = order;
    bh.padded = padding;
    bh.unused1 =0;
    bh.rsize = rsize;
    bh.unused2 = 0;
    address->header = bh;
    return (void*) address;
}
/*
now we have the sentinal node of the list
*/
bud_free_block *sentinel = &free_list_heads[index];
while( (sentinel->next == sentinel)&&(sentinel->prev == sentinel)){
    index = index +1;
sentinel = &free_list_heads[index];
if(index == (ORDER_MAX - ORDER_MIN)-1){

    if(bud_sbrk()== (void*)-1){
                return NULL;
            }
     /* split the block, place half in an appropriate list,
    allocate half */
            int max = ORDER_MAX -1;
            bud_free_block * address = bud_heap_end();
            for(int i = max; i >= order; i--){
                int index = i - ORDER_MIN;
                int half = ORDER_TO_BLOCK_SIZE(i)/2;

                 bud_header bh;
                 bh.allocated = 0;
                 bh.order = i;
                 bh.padded = 0;
                 bh.unused1 =0;
                 bh.rsize = 0;
                 bh.unused2 = 0;
                 bud_free_block *sentinel = &free_list_heads[index];
                 bud_free_block *next = sentinel->next;

                 address->header = bh;
                 address->next = next;
                 address->prev = sentinel;
                 sentinel->next = address;
                 (*next).prev = address;
                  address = (void*)address - half;


                 if (i== order)
                 {
                     address = (void*)address - half;
                      int index = i - ORDER_MIN;
                      int half = ORDER_TO_BLOCK_SIZE(i)/2;
                     bud_header bh;
                     bh.allocated = 0;
                     bh.order = i;
                     bh.padded = 0;
                     bh.unused1 =0;
                     bh.rsize = 0;
                     bh.unused2 = 0;
                     bud_free_block *sentinel = &free_list_heads[index];
                     bud_free_block *next = sentinel->next;

                     address->header = bh;
                     address->next = next;
                     address->prev = sentinel;
                     sentinel->next = address;
                     (*next).prev = address;
                 }


            }





}

}
/* we found a block and we didnt have to sbrk */
if(index != (ORDER_MAX - ORDER_MIN)-1){
    if (index!= oldIndex)
    {
        /* code */

    sentinel = &free_list_heads[index];
    bud_free_block *nodeToRemove = sentinel;
    nodeToRemove = nodeToRemove->next;
    sentinel->next = nodeToRemove->next;
    (nodeToRemove->next)->prev = sentinel;

    bud_free_block * address = nodeToRemove;
    int oldOrder = order;
    int order = index + ORDER_MIN;
    for (int i = order-1; i >= oldOrder; i--)
    {
                int index = i - ORDER_MIN;
                int half = ORDER_TO_BLOCK_SIZE(i);
                address = (void*)address + half;
                 bud_header bh;
                 bh.allocated = 0;
                 bh.order = i;
                 bh.padded = 0;
                 bh.unused1 =0;
                 bh.rsize = 0;
                 bh.unused2 = 0;
                 bud_free_block *sentinel = &free_list_heads[index];
                 bud_free_block *next = sentinel->next;

                 address->header = bh;
                 address->next = next;
                 address->prev = sentinel;
                 sentinel->next = address;
                 (*next).prev = address;


                 address = (void*)address - half;

                  if (i== oldOrder)
                 {

                      int index = i - ORDER_MIN;
                      int half = ORDER_TO_BLOCK_SIZE(i);
                     bud_header bh;
                     bh.allocated = 0;
                     bh.order = i;
                     bh.padded = 0;
                     bh.unused1 =0;
                     bh.rsize = 0;
                     bh.unused2 = 0;
                     bud_free_block *sentinel = &free_list_heads[index];
                     bud_free_block *next = sentinel->next;

                     address->header = bh;
                     address->next = next;
                     address->prev = sentinel;
                     sentinel->next = address;
                     (*next).prev = address;

                 }
    }
    index = oldIndex;
    /* if oldindex = index we didnt have to get the next highest size
    so allocate the next free block in this list*/
    sentinel = &free_list_heads[index];
    nodeToRemove = sentinel;
    nodeToRemove = nodeToRemove->next;
    sentinel->next = nodeToRemove->next;
    (nodeToRemove->next)->prev = sentinel;
    nodeToRemove->prev = NULL;
    nodeToRemove->next = NULL;
    nodeToRemove -> header.allocated =1;
    nodeToRemove -> header.padded = padding;
    nodeToRemove -> header.rsize = rsize;
    return (void*)nodeToRemove;
    }
}
/* now we have the index of the free block list*/
index = oldIndex;
    /* if oldindex = index we didnt have to get the next highest size
    so allocate the next free block in this list*/
sentinel = &free_list_heads[index];
    bud_free_block *nodeToRemove = sentinel;
    nodeToRemove = nodeToRemove->next;
    sentinel->next = nodeToRemove->next;
    (nodeToRemove->next)->prev = sentinel;
    nodeToRemove->prev = NULL;
    nodeToRemove->next = NULL;
    nodeToRemove -> header.allocated =1;
    nodeToRemove -> header.padded = padding;
    nodeToRemove -> header.rsize = rsize;
    return (void*)nodeToRemove;




    /* split the block, place half in an appropriate list,
    allocate half */


}

void *bud_malloc(uint32_t rsize) {


int oldRsize = rsize;

    if (rsize == 0){
        errno =EINVAL;
        return NULL;
    }
    if(rsize > (MAX_BLOCK_SIZE - sizeof(bud_header))){
        errno =EINVAL;
        return NULL;

    }
    /*
    rsize must be included to add the header size
    */
    rsize = rsize + sizeof(bud_header);
    int order = 0;
    int min = ORDER_MIN;
    int padding = 0;

    /*
    if the rsize anded with the size = 0 then the block is big enough
    */
    while(order == 0){

        int size = (ORDER_TO_BLOCK_SIZE(min));
            if(size> rsize){
                order = min;
                padding++;
            }
            /* if the rsize & the size = the size then the rsize + header is
            exactly the block size and we need no padding */
            if(rsize == size){
                order = min;
                padding =0;
            }
            min++;

    }

bud_free_block * freeBlock = findBlock(order,padding,oldRsize);
if(freeBlock == NULL){
    return NULL;
}
    void* addressOfPayload = freeBlock;
    addressOfPayload = addressOfPayload + sizeof(bud_header);

return addressOfPayload;
}

void *bud_realloc(void *ptr, uint32_t rsize) {
    int oldRsize = rsize;
    if(rsize == 0){
        bud_free(ptr);
        return NULL;
    }
    if(rsize > (MAX_BLOCK_SIZE - sizeof(bud_header))){
        errno =EINVAL;
        return NULL;
    }

ptr = (void*)ptr - sizeof(bud_header);

if(ptr< bud_heap_start()){
        /* call abort */
        abort();
    }
    if(ptr > bud_heap_end()){
        /* call abort */
        abort();

    }
    if(((uintptr_t)ptr & (uintptr_t)0x00000004) != 0){
        /* call abort */

        abort();
    }
    bud_header *hd = ptr;
    if((hd -> order) < ORDER_MIN || ((hd -> order)> ORDER_MAX)){
        /* call abort */
        abort();
    }
    if((hd -> padded)==0){
        int blockSize = ORDER_TO_BLOCK_SIZE(hd -> order);
        int actualSize = (hd -> rsize) + sizeof(bud_header);
        if(blockSize != actualSize){
            /*abort */
            abort();

        }
    }
    if((hd -> padded)==1){
        int blockSize = ORDER_TO_BLOCK_SIZE(hd -> order);
        int actualSize = (hd -> rsize) + sizeof(bud_header);
        if(blockSize == actualSize){
            /*abort */
            abort();

        }
    }
    int size = hd -> rsize;
    size = size +sizeof(bud_header);
    int actualSize = ORDER_TO_BLOCK_SIZE(hd-> order);
    if (size> actualSize)
    {
        /* abort */
        abort();
    }
    int calculatedOrder = 0;
     int min = ORDER_MIN;
    while(calculatedOrder == 0){
        int sizeOfOrder = (ORDER_TO_BLOCK_SIZE(min));
            if(sizeOfOrder> size){
                calculatedOrder = min;

            }
            /* if the rsize & the size = the size then the rsize + header is
            exactly the block size and we need no padding */
            if(size == sizeOfOrder){
                calculatedOrder = min;

            }
            min++;
    }
    if(calculatedOrder != hd-> order){
        abort();
    }
    rsize = rsize + sizeof(bud_header);
    int order = 0;
    min = ORDER_MIN;
    int padding = 0;

    /*
    if the rsize anded with the size = 0 then the block is big enough
    */
    while(order == 0){

        int sizeOfOrder = (ORDER_TO_BLOCK_SIZE(min));
            if(sizeOfOrder> rsize){
                order = min;
                padding++;
            }
            /* if the rsize & the size = the size then the rsize + header is
            exactly the block size and we need no padding */
            if(rsize == sizeOfOrder){
                order = min;
                padding =0;
            }
            min++;

    }
    /* now we have the order */
    /* if the new order = the order of the block  then we just return the same block */
   bud_header* header = (bud_header*) ptr;
    if(order == header->order ){
        return ptr;
    }
    /* if the new block is bigger then we must allocate memory for new block and copy payload data */
    if(order > header -> order){

        bud_free_block * freeBlock = findBlock(order,padding,oldRsize);
        ptr = ptr + sizeof(bud_header);
        void * src = ptr;
        void * dest = (void*) freeBlock + sizeof(bud_header);
        memcpy(dest,src, header->rsize);

        bud_free(ptr);
        return dest;
    }
    /* if the new block is smaller return the ptr and if it can be split in half do it */
    if(order < header -> order){
        bud_free_block * address = (bud_free_block *) ptr;
        for(int i = (address ->header.order)-1; i >= order; i--){
            int index = i - ORDER_MIN;
                int half = ORDER_TO_BLOCK_SIZE(i);
                 bud_header bh;
                 bh.allocated =0;
                 bh.order = i;
                 bh.padded = 0;
                 bh.unused1 =0;
                 bh.rsize = 0;
                 bh.unused2 = 0;
                 bud_free_block *sentinel = &free_list_heads[index];
                 bud_free_block *next = sentinel->next;

                 address->header = bh;
                 address->next = next;
                 address->prev = sentinel;
                 sentinel->next = address;
                 (*next).prev = address;


                 address = (void*)address - half;
                  if (i== order)
                 {

                      int index = i - ORDER_MIN;
                      int half = ORDER_TO_BLOCK_SIZE(i);
                     bud_header bh;
                     bh.allocated = 1;
                     bh.order = i;
                     bh.padded = padding;
                     bh.unused1 =0;
                     bh.rsize = oldRsize;
                     bh.unused2 = 0;


                     address->header = bh;

                 }
    }
        return (void*)address + sizeof(bud_header);
        }
    return NULL;
    }




void bud_free(void *ptr) {
    // make the pointer point back to the beggining of the block

ptr = (void*)ptr - sizeof(bud_header);

if(ptr< bud_heap_start()){
        /* call abort */
        abort();
    }
    if(ptr > bud_heap_end()){
        /* call abort */
        abort();

    }
    if(((uintptr_t)ptr & (uintptr_t)0x00000004) != 0){
        /* call abort */

        abort();
    }
    bud_header *hd = ptr;
    if((hd -> order) < ORDER_MIN || ((hd -> order)> ORDER_MAX)){
        /* call abort */
        abort();
    }
    if((hd -> padded)==0){
        int blockSize = ORDER_TO_BLOCK_SIZE(hd -> order);
        int actualSize = (hd -> rsize) + sizeof(bud_header);
        if(blockSize != actualSize){
            /*abort */
            abort();

        }
    }
    if((hd -> padded)==1){
        int blockSize = ORDER_TO_BLOCK_SIZE(hd -> order);
        int actualSize = (hd -> rsize) + sizeof(bud_header);
        if(blockSize == actualSize){
            /*abort */
            abort();

        }
    }
    int rsize = hd -> rsize;
    int actualSize = ORDER_TO_BLOCK_SIZE(hd-> order);
    if (rsize> actualSize)
    {
        /* abort */
        abort();
    }
    int position;
    bud_header* buddyAddress = (bud_header *)((uintptr_t)ptr ^ (uintptr_t)actualSize);
    if ((uintptr_t)buddyAddress > (uintptr_t)ptr)
    {
        position = 0;
    }
    else{
        position = 1;
    }

        coalesce(ptr, buddyAddress,position);


    return;
}
