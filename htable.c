#include "htable.h" 
	/*  
		* a simple hashtable which maps 32 bit addresses to a void *  	

		* there are 2^KEY_BITS unique hashes  

		* there is a table BLOCK for each unique hash. this block 
			contains BLOCK_DEPTH_1550 SLOTS. Each SLOT is a table 
			entry.  

		* to acomodate more than BLOCK_DEPTH addresses with a given 
			hash, a new block may be alloced and a linked list of blocks formed 
	 
	*/

//unsigned next_handle = 0; // unique handles  

htable_header *  htable_init( unsigned char key_bits/*, unsigned char block_depth*/ ) { 
	if ( key_bits > 32 ) { 
		fprintf(stderr, "Invalud key bits argument to htable_init\n");  
		return NULL; 
	} 
	// "there is a block for each unique hash, and 2^key_bits unique hash"  
	unsigned block_ct = ( 1 << key_bits ); 

	size_t table_buf_sz = block_ct * sizeof(htable_block); 	
	//printf("tbuf sz: %zu\n", table_buf_sz); 

	htable_header * ret = malloc( sizeof(htable_header) + table_buf_sz); 
	if ( !ret ) { 
		fprintf(stderr, "htable_init: malloc failed with errno %i\n", errno); 
		return NULL; 
	} 

	memset( ret, 0, sizeof(htable_header) + table_buf_sz ); 

	//	htable_header * ret = mmap(NULL, sizeof(htable_header) + table_buf_sz,
	//		(PROT_READ | PROT_WRITE), -1, MAP_ANON, 0);  
	//
	//	if ( ret == MAP_FAILED ) { 
	//		fprintf(stderr, "htable_init: mmap failed with errno %i\n", errno); 
	//		return NULL; 
	//	} 

	ret->key_bits = key_bits; 
	ret->key_mask = block_ct - 1; 
	ret->block_ct = block_ct; 
	ret->tb_buffer = (htable_block *) (ret+1); 

	ret->keys = gqueue_init(); 
	return ret; 
}  

int htable_set( htable_header * table,  uint32_t addr, void * value ) { 
	if ( !value ) { 
		fprintf(stderr, "htable: Key null not allowed\n"); 	
		return -1; 
	} 	
	uint32_t key = addr & table->key_mask; 	
	//assert( key < 
	htable_block * block = table->tb_buffer + key; 	
	
	// traverse block for match or empty  
	int overwrite = 0; 
	int i; 
	htable_entry * c_ent; 

	_trav_block:; 
	c_ent = block->entries; 	
	i = BLOCK_DEPTH ;  
	
	while ( i && c_ent->value ) { 
		if ( c_ent->addr == addr ) { 
			overwrite=1; 	
			break; 
		} 
		c_ent++; 
		i--;  
	}  

	if ( !i ) { // reached end of block 	
		if ( block->next ) { // if another block in ll, traverse that one  
			block = (htable_block *) block->next; 
			goto _trav_block; 
		} else { // else alloc a new block with new entry as fist elem  
			htable_block * new_b = calloc(1, sizeof(htable_block)); 
			if ( ! new_b ) { 
				fprintf(stderr, "htable: calloc failed with errno %i\n", errno); 
				exit(0); 
			} 
			block->next = (void *) new_b;  
			c_ent = new_b->entries; 
			table->block_ct++; 
		}  
	} 
	
	c_ent->addr = addr; 
	c_ent->value = value; 

	if ( !overwrite ) { // add this new key to keys list 
		gqueue_push(table->keys, addr); 	
	} 

	return overwrite; 	
} 

void * htable_get( htable_header * table, uint32_t addr ) { 
	uint32_t key = addr & table->key_mask; 	
	//assert( key < 
	htable_block * block = table->tb_buffer + key; 	

	// traverse block for match  
	htable_entry * c_ent; 	
	int i; 

	_trav_block:; 
	i = BLOCK_DEPTH ;  
	c_ent = block->entries;
	
	while ( i && c_ent->value ) { 
		if ( c_ent->addr == addr ) { 
			return c_ent->value; 
		} 
		c_ent++; 
		i--;  
	}  

	if ( !i &&block->next ) { // reached end of block with another to traverse	
		block = (htable_block *) block->next; 
		goto _trav_block; 
	} 
	// else no match found 
	return 0; 
} 

