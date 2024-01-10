#ifndef RUNTIME
#define RUNTIME

#include <stdlib.h>
#include <assert.h>

#define BLOCKSIZE 63
typedef char * BLST_DTYPE;

typedef struct{
    BLST_DTYPE items[BLOCKSIZE];
    void * next;
} block;

typedef struct{
    block * head;
    block * tail;
    unsigned elem_ct;
} block_list;

block_list * blst_construct();
void blst_destruct(block_list *);
void blst_append(block_list *, BLST_DTYPE);
BLST_DTYPE blst_geti(block_list *, unsigned); 
BLST_DTYPE * blst_as_array(block_list *);

#endif
