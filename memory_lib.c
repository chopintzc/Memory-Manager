
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
/* #include "memory_lib.h" */

typedef struct{
   void* pointer;
   unsigned int size;
   unsigned int used;
   char side[8];
} Block;


void *get_memory(int size);
void release_memory(void *p);
void *grow_memory(int size, void *p);
void *pregrow_memory(int size, void *p);
int start_memory(int size);
void end_memory(void);
void print_memory();

/* Global variables */
static int memory_size = 0;
static int free_space = 0;
static void* memory;
static int partition = 0;
static int boundary[15] = {1,2,4,8,16,32,64,128,256,512,1024,2048,4096,8192,16384};

/* To find a best fit memory partition */
Block* find_hole(int size){
   Block* best_block = NULL;
   Block* current_block = memory;
    
   /* first, we have to find a un-used memory partition */
   while(current_block->used != 0 || current_block->size < size+sizeof(Block)){
      if(((void *)current_block+(current_block->size)) >= memory+memory_size){
         return NULL;
      }
      current_block = (void *)current_block+(current_block->size);
   }
   best_block = current_block;
    
    /* find the next memory partition */
   while(((void *)current_block+(current_block->size))<memory+memory_size){
      current_block = (void *)current_block+(current_block->size);
      /* if the next memory partition exists, it not being used and the size is appropriate, it is the best_block so far*/
      if(current_block->size < best_block->size && current_block->size >= size+sizeof(Block) && current_block->used == 0){
         best_block = current_block;
      }
   }
    
   return best_block;
}

/* split a memory partition */
void split_block(Block* current) {
   Block* new_block = (void *)current+((current->size)/2);
   new_block->pointer = (void *)new_block+sizeof(Block);
   new_block->size = (current->size)/2;
   new_block->used = 0;
   strcpy(new_block->side, current->side);
   strcat(new_block->side, "2");
    
   current->size = (current->size)/2;
   strcat(current->side, "1");
}

/* reversely split a memory partition */
void reverse_split_block(Block* current) {
   Block* new_block = (void *)current+((current->size)/2);
   new_block->pointer = (void *)new_block+sizeof(Block);
   new_block->size = (current->size)/2;
   new_block->used = current->used;
   strcpy(new_block->side, current->side);
   strcat(new_block->side, "2");
    
   current->used = 0;
   current->size = (current->size)/2;
   strcat(current->side, "1");
}

void *get_memory(int size){
    
   /* if the memory not initialized or get memory size is invalid, return NULL */
   if(free_space==0 || size<=0){
      printf("invalid size!\n");
      return NULL;
   }
    
   /* if the new memory partition size is bigger than free memory size, return NULL */
   if(size > free_space){
      printf("No free space!");
      return NULL;
   }
    
   /* Thread safe: no more than one thread can find the hole or add new block at the same time */
   Block* fit = find_hole(size);
    
   /* if there is no hole fit the new memory partition, return NULL */
   if(fit == NULL){
      printf("No fit!\n");
      return NULL;
   }
    
   /* find the best fit free space */
   int fit_size = fit->size;
    
   while((size+sizeof(Block))*2<=fit_size){
      /* if size of the free block is bigger than the new memory partition size *2, split it */
      split_block(fit);
      fit_size = fit->size;
   }
    
   free_space -= size+sizeof(Block);
    
   /* mark the memory space have been used */
   partition++; //invoke a new memory partition ID
    
   fit->used = partition;
    
    
   return fit->pointer;
}

/* To find the memory partition in the binary tree with given pointer */
Block* find_block_list(void *p){
   Block *current_block = memory;
    
   while((void *)current_block < memory+memory_size){
      if(current_block->pointer == p){
         return current_block;
      }
        
      current_block = (void *)current_block+(current_block->size);
   }
    
   return NULL;
}

/* Try to combine the memory partition */
void *combine_block (Block *current){
   if(current->side[strlen(current->side)-1] == '1'){
      Block *next_block = (void *)current+(current->size);
      next_block->side[0] = '\0';
      current->size = (current->size)*2;
      current->side[strlen(current->side)-1] = '\0';
      
      return current;
   }
   else{
      current->side[0] = '\0';
      Block *previous_block = (void *)current-(current->size);
      previous_block->size = (previous_block->size)*2;
      previous_block->side[strlen(previous_block->side)-1] = '\0';
        
      return previous_block;
   }
}

void release_memory(void *p){
   Block *current = find_block_list(p);
   Block *next_block = NULL;
   Block *previous_block = NULL;
    
   if(current == NULL){
      return;
   }
    
   /* release current memory partition */
   current->used = 0;
    
   free_space += current->size;
    
   /* check if the current memory partition can be combine with its neighbour, combine them */
   while(1){
      /* if current partition is on the LEFT_SIDE, the next partition exist, same size and not being used, combine them */
      if(strlen(current->side) > 0 && current->side[strlen(current->side)-1] == '1' && ((void *)current+(current->size)) < memory+memory_size){
         next_block = (void *)current+(current->size);
         if(next_block->used == 0 && next_block->size == current->size){
            current = combine_block(current);
         }
         else{
            break;
         }
      }
        /* if current partition is on the RIGHT_SIDE, the previous partition exist, same size and not being used, combine them */
      else if(strlen(current->side) > 0 && current->side[strlen(current->side)-1] == '2' && ((void *)current-(current->size)) >= memory){
         previous_block = (void *)current-(current->size);
         if(previous_block->used == 0 && previous_block->side[strlen(previous_block->side)-1] == '1' && previous_block->size == current->size){
            current = combine_block(previous_block);
         }
         else{
            break;
         }
      }
      else{
         break;
      }
   }
}


void *grow_memory(int size, void *p){
   int i;
   Block *current = find_block_list(p);
    
   /* if we cannot found the memory partition by *p, return NULL */
   if(current == NULL){
      return NULL;
   }
    
   /* calculate the new binary buddies memory partition size (2^i) */
   int new_size = size+sizeof(Block);
   i=0;
   while (sizeof(boundary)/sizeof(int) > i && boundary[i] < new_size){
      i++;
   }
   if (sizeof(boundary)/sizeof(int) > i){
      new_size = boundary[i];
   }
   else {
      return NULL;
   }
    
   /* if the new memory partition size is bigger than old size, try to relocate the partition if the current partition cannot be grown in-place. */
   if(new_size > (current->size)){
      /* first, we have to backup the current memory status. we will try to release current memory partition, combine the partition if it is possible, and then try to find a new location that fit the new partition size. If we cannot find an appoprate space, we will recover the old memory status from the backup. */
      /* to count how many memory partitions so far */
      int count = 0;
      Block *current_block = memory;
      while((void *)current_block < memory+memory_size){
         count++;
         current_block = (void *)current_block+(current_block->size);
      }
      /* create an array and backup the current memory information */
      Block backup[count];
      Block *next_block = NULL;
      current_block = memory;
      int i;
      for(i=0; i<count; i++){
         backup[i].pointer = current_block->pointer;
         backup[i].size = current_block->size;
         backup[i].used = current_block->used;
         strcpy(backup[i].side, current_block->side);
         current_block = (void *)current_block+(current_block->size);
      }
      
      /* release current memory partition */
      current->used = 0;
      free_space += current->size; 
      
      
      /* check if the current memory partition can be combine with its neighbour, combine them */
      while(1){
         /* if current partition is on the LEFT_SIDE, the next partition exist, same size and not being used, combine them */
         if(strlen(current->side) > 0 && current->side[strlen(current->side)-1] == '1' && ((void *)current+(current->size)) < memory+memory_size && current->size < new_size){
            next_block = (void *)current+(current->size);
            if(next_block->used == 0 && next_block->size == current->size ){
               current = combine_block(current);
            }
            else{
               break;
            }
         }
         else{
            break;
         }
      }
      
      /* if current partition can be expanded */
      if (current->size >= new_size){
         current->used = 1;
         free_space -= current->size;
         return current->pointer;
      }
      /* if need to relocate to another partition */
      else {
         current_block = memory;
         for(i=0; i<count ; i++){
            current_block->pointer = backup[i].pointer;
            current_block->size = backup[i].size;
            current_block->used = backup[i].used;
            strcpy(current_block->side, backup[i].side);
            current_block = (void *)current_block+(current_block->size);
         }
         
         /* release current memory partition */
         current->used = 0;
      
         /* try to find a new fit space with the new size */  
         partition--;   
         void *new_block = get_memory(size);
         
         /* if we relocate the memory partition successful, copy the contents of memory from the old partition to the new partition */
         if(new_block != NULL){
            memcpy(new_block, (current->pointer), (current->size));
            release_memory(current->pointer);  
            return new_block;
         }
         /* if we cannot relocate the partition */
         else{   
            release_memory(current->pointer);
            return NULL;
         }
      }
   }
   /* If the new memory partition size is smaller than what is currently allocated, then space is truncated */
   else if(new_size < (current->size)){

      /* split the current partition until the partition size reached the new size */
      while(current->size >= 2*new_size){
         split_block(current);
      }
        
      printf("returned address %p\n", current->pointer);
      return (void *)current->pointer;
   }
   /* if the new size is equal to the old size, return NULL */
   else{
      return NULL;
   }
}

void *pregrow_memory(int size, void *p){
   int i;
   Block *current = find_block_list(p);
   Block *original = current;
   
   /* if we cannot found the memory partition by *p, return NULL */
   if(current == NULL){
      return NULL;
   }
    
   /* calculate the new binary buddies memory partition size (2^i) */
   int new_size = size+sizeof(Block);
   i=0;
   while (sizeof(boundary)/sizeof(int) > i && boundary[i] < new_size){
      i++;
   }
   if (sizeof(boundary)/sizeof(int) > i){
      new_size = boundary[i];
   }
   else {
      return NULL;
   }
    

   /* if the new memory partition size is bigger than old size, try to relocate the partition if the current partition cannot be grown in-place. */
   if(new_size > (current->size)){
      /* first, we have to backup the current memory status. we will try to release current memory partition, combine the partition if it is possible, and then try to find a new location that fit the new partition size. If we cannot find an appoprate space, we will recover the old memory status from the backup. */
      /* to count how many memory partitions so far */
      int count = 0;
      Block *current_block = memory;
      while((void *)current_block < memory+memory_size){
         count++;
         current_block = (void *)current_block+(current_block->size);
      }
      /* create an array and backup the current memory information */
      Block backup[count];
      Block *previous_block = NULL;
      current_block = memory;
      int i;
      for(i=0; i<count; i++){
         backup[i].pointer = current_block->pointer;
         backup[i].size = current_block->size;
         backup[i].used = current_block->used;
         strcpy(backup[i].side, current_block->side);
         current_block = (void *)current_block+(current_block->size);
      }
      
      /* release current memory partition */
      current->used = 0;
      free_space += current->size; 
      
      /* check if the current memory partition can be combine with its neighbour, combine them */
      while(1){
         /* if current partition is on the RIGHT_SIDE, the next partition exist, same size and not being used, combine them */
         if(strlen(current->side) > 0 && current->side[strlen(current->side)-1] == '2' && ((void *)current-(current->size)) >= memory && current->size < new_size){
            previous_block = (void *)current-(current->size);
            if(previous_block->used == 0 && previous_block->side[strlen(previous_block->side)-1] == '1' && previous_block->size == current->size){
               current = combine_block(previous_block);
            }
            else{
               break;
            }
         }
         else{
            break;
         }
      }
      
      /* if current partition can be expanded */
      if (current->size >= new_size){
         current->used = 1;
         free_space -= current->size;
         return (void *)current->pointer;
      }
      /* if need to relocate to another partition */
      else {
         current_block = memory;
         for(i=0; i<count ; i++){
            current_block->pointer = backup[i].pointer;
            current_block->size = backup[i].size;
            current_block->used = backup[i].used;
            strcpy(current_block->side, backup[i].side);
            current_block = (void *)current_block+(current_block->size);
         }
         
         /* release current memory partition */
         original->used = 0;
      
         /* try to find a new fit space with the new size */  
         partition--;   
         void *new_block = get_memory(size);
         
         /* if we relocate the memory partition successful, copy the contents of memory from the old partition to the new partition */
         if(new_block != NULL){
            memcpy(new_block, (original->pointer), (original->size));
            release_memory(original->pointer);  
            return (void *)new_block;
         }
         /* if we cannot relocate the partition */
         else{   
            release_memory(original->pointer);
            return NULL;
         }
      }
   }
   /* If the new memory partition size is smaller than what is currently allocated, then space is truncated */
   else if(new_size < (current->size)){

      /* truncate the memory content */
      memcpy((void *)current+((current->size)- new_size), (void *)current, new_size);
      /* split the current partition until the partition size reached the new size */
      while(current->size >= 2*new_size){
         reverse_split_block(current);
         current = (void *)current+(current->size);
      }
      
      printf("returned address %p\n", current->pointer);
      return (void *)current->pointer;
   }
   /* if the new size is equal to the old size, return NULL */
   else{
      return NULL;
   }

}


int start_memory(int size){
   int i;
   printf("Memory partition header size: %lu\n", sizeof(Block));
    
   /* if the memory already initialized or the size is smaller than the memory header size, return false */
   if(memory_size!=0 || size<sizeof(Block)){
      return 0;
   }
    
   /* calculate the memory_size */
   i=0;
   while (sizeof(boundary)/sizeof(int) > i && boundary[i] <= size){
      i++;
   }
   memory_size = boundary[i-1];
   
   /* calculate the free space without a header */
   free_space = memory_size-sizeof(Block);
    
   /* Allocate memory */
   memory = malloc(size);
    
   if(memory == NULL){
      return 0;
   }
    
   Block *memory_list = memory;
    
   memory_list->pointer = (void *)memory+sizeof(Block);
   memory_list->size = memory_size;
   memory_list->used = 0;
   memory_list->side[0] = '1';
        
   printf("memory address: %p\n", memory);
    
   return 1;
}

void end_memory(void){
   free(memory);
}

/* This method print the memory_list. It can be used for debug */
void print_memory(){
   printf("memory status:\n");
   printf("----------\n");
    
   Block *current = memory;
    
   while((void *)current<(memory+memory_size)){
      if(current->used == 0){
         printf("free size: %d\n", current->size);
         printf("header address: %p\n", current);
         printf("free space address: %p\n", current->pointer);
      }
      else{
         printf("P%d\n", current->used);
         printf("total size:%d\n", current->size);
         printf("header address: %p\n", current);
         printf("content address: %p\n", current->pointer);
      }
      printf("----------\n");
      current = (void *)current+(current->size);
   }
}

int main(){
   int start = 0;
   printf("start_memory: ");
   scanf("%i", &start);
    
   if(start_memory(start)==0){
      printf("Start Memory Error\n");
      return 1;
   }
   print_memory();
    
   int number;
   char command[10];
   void *pointer = NULL;
   while(1){
      printf("> ");
      scanf("%s", command);
        
      if(strcmp(command, "get")==0){
         scanf("%d", &number);
         void* newPointer = get_memory(number);
            
         if(newPointer==NULL){
            printf("Not enough Memory Space\n");
         }
         else{
            print_memory();
            printf("Placed new memory partition successful\nAddress: %p\n", newPointer);
         }
      }
      else if(strcmp(command, "release")==0){
         scanf("%p", &pointer);
         release_memory(pointer);
         print_memory();
      }
      else if(strcmp(command, "grow")==0){
         scanf("%d", &number);
         scanf("%p", &pointer);
         grow_memory(number, pointer);
         print_memory();
      }
      else if(strcmp(command, "pregrow")==0){
         scanf("%d", &number);
         scanf("%p", &pointer);
         pregrow_memory(number, pointer);
         print_memory();
      }
      else if(strcmp(command, "end")==0){
         end_memory();
         break;
      }
      else{
         printf("Invalid Command\n");
      }
   }
    
   return 0;
}