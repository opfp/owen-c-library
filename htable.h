#ifndef HTABLE 
#define HTABLE 

#include <stdlib.h> 
#include <stdio.h> 
#include <sys/mman.h>  
#include <errno.h> 
#include <assert.h>
#include <string.h> 
#include "gqueue.h"

#define BLOCK_DEPTH 4 // number of entries for a unique hash  

typedef struct { 
	uint32_t addr; 
	void * value;  
} htable_entry;   

typedef struct { 
	htable_entry entries[BLOCK_DEPTH]; 
	void * next; 
} htable_block; 

typedef struct { 
	unsigned key_bits; 
	uint32_t key_mask; 
	unsigned block_ct; 
	htable_block * tb_buffer; 
	gqueue_obj * keys; 
} htable_header; 

htable_header * htable_init( unsigned char key_bits ); 
int htable_set( htable_header * table, uint32_t addr, void * value ); 
void * htable_get(htable_header * table,uint32_t addr);  

#endif 
