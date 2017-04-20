
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