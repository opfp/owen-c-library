#include "htable.h"  
#include <time.h> 

#define TST_SZ 849688 
//#define TST_SZ 700000 

int main(){ 
	//uint32_t keys[TST_SZ]; 
	//void * values[TST_SZ]; 

	uint32_t * keys = malloc(TST_SZ*sizeof(int)); 
	void * * values = malloc(TST_SZ*sizeof(void *));  
	assert (keys && values); 

	htable_header * table = htable_init(10); 	
	assert(table); 	

	printf("blocks: %u, total size: %zu\n", table->block_ct, 
		table->block_ct * sizeof(htable_block) );  

 	srand(time(NULL)); 	

	uint32_t c_k; 
	void * c_v;  
	int ovr; 

	int expected_unique_keys = TST_SZ; 

	for ( int i = 0; i < TST_SZ; ++i ) { 
		c_k = rand(); 		
		c_v = (void *) ( ((long) rand()) * ( 1 << (rand() % 16) ) );  	
		ovr = htable_set( table, c_k, c_v ); 
		if ( ovr  == -1 ) { 
			fprintf(stderr, "htable_set failed\n"); 
			return -1; 
		} else if ( ovr == 1 ) { 
			expected_unique_keys--; 	
		}  	
		keys[i] = c_k; 
		values[i] = c_v; 
	//	if ( !(i%64) ) 
	//		printf("table[%x] := %p\n",c_k, c_v); 
	}  

	printf("blocks: %u, total size: %zu\n", table->block_ct, 
		table->block_ct * sizeof(htable_block) );  

	void * t_r; 

	for ( int i = 0; i < TST_SZ; ++i ) { 
		c_k = keys[i]; 
		c_v = values[i]; 
		t_r = htable_get(table, c_k);  

		if ( t_r != c_v ) { 
			fprintf(stderr, "table[%x] returns %p, mismatch, should be %p\n", 
				c_k, t_r, c_v );  
		} 		

		//if ( !(i%64) ) 
		//	printf("table[%x] -> %p\n",c_k, c_v); 

	} 

	unsigned num_keys; 
	gqueue_as_array(table->keys, &num_keys); 		
	printf("Table has %d unique keys, expected %d\n", num_keys,expected_unique_keys); 

} 
