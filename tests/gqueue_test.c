#include "../gqueue.h" 
#include "stdio.h" 

int main() { 
	uint32_t i = GQUEUE_BLOCK_SZ*3+1; 
	gqueue_obj * queue = gqueue_init(); 

	printf("Push and pop from %d to %d\n", 0, i ); 

	for( int j = 0; j < i; ++j ) { 
		gqueue_push(queue, j); 	
	} 
	
	for( int j = 0; j < i; ++j ) { 
		printf("\t%d \n", gqueue_pop(queue) ); 	
	} 
	
	printf("Push and pop_back from %d to %d\n",0, i ); 
	
	for( int j = 0; j < i; ++j ) { 
		gqueue_push(queue, j); 	
	} 

	for( int j = 0; j < i; ++j ) { 
		printf("\t%d \n", gqueue_pop_back(queue) ); 	
	} 

	for( int j = 0; j < i; ++j ) { 
		//printf("%d\n", j);
		gqueue_push(queue, j+i); 	
	} 

	unsigned sz; 
	uint32_t * rv = gqueue_as_array(queue, &sz); 
	printf("%d elems, %d blocks \n", sz, queue->node_ct ); 
	
	while ( sz-- ) { 
		printf( " %d \n", *rv++ ); 
	}
} 

