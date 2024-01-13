#ifndef GQUEUE 
#define GQUEUE 

#include <stdio.h> 
#include <stdlib.h>
#include <assert.h> 
#include <string.h> 

#define GQUEUE_BLOCK_SZ 8 

typedef struct { 
	unsigned short size; 
	unsigned short s_i; 
	uint32_t elems[GQUEUE_BLOCK_SZ]; 		
	void * next; 
} gqueue_node;  

typedef struct { 
	gqueue_node * head; 	
	gqueue_node * tail; 	
	unsigned node_ct; 
} gqueue_obj; 

gqueue_obj * gqueue_init(); 
//int gqueue_pop( gqueue_node * * head); 
//int gqueue_poll( gqueue_node * head); 
//void gqueue_push(gqueue_node * head, int value);  
uint32_t gqueue_pop( gqueue_obj * queue); 
uint32_t gqueue_poll( gqueue_node * node); 
void gqueue_push(gqueue_obj * queue, uint32_t value);  
uint32_t * gqueue_as_array( gqueue_obj * queue, unsigned * elem_ct_wb); 

#endif 
