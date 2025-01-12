#include "gqueue.h" 

gqueue_obj * gqueue_init() { 
	//gqueue_obj * new_queue = calloc(sizeof(gqueue_obj) + sizeof(gqueue_node),1); 	
	//gqueue_node * new_node = (gqueue_node *) (new_queue+1); 	
	gqueue_obj * new_queue = calloc(sizeof(gqueue_obj) ,1); 	
	gqueue_node * new_node = calloc(sizeof(gqueue_node),1); 	
	//gqueue_obj new_queue; 
	//gqueue_node * new_node = calloc(sizeof(gqueue_node) , 1); 

	new_queue->head=new_node; 
	new_queue->tail=new_node; 

	new_queue->node_ct =1; 
	return new_queue; 
} 

uint32_t gqueue_pop( gqueue_obj * queue ){ 	 	
	//gqueue_obj * queue = *queue_i; 	
	gqueue_node * head = queue->head; 
	if ( head->size == 0 ) { 
		fprintf(stderr, "gqueue.pop: cannot pop from empty queue\n"); 
		exit(-1); 
	} 
	uint32_t ret = head->elems[head->s_i];   
	head->s_i++; 
	head->size--; 

	if ( !head->size && head->next) { // if we popped off the last node in this block  
		queue->head = head->next; // make the head the next block by chaning head * in user's mem  
		queue->head->prev = NULL;  
		queue->node_ct--; 

		if ( ! queue->free_node ) {  
			queue->free_node = head; 
		  // node to be recycled 
		  head->s_i = 0; 
		  head->size = 0;  
		 	head->next = NULL; 
			head->prev = NULL; 
		} else { 
			free(head); 
	  }													
		// mark this as a free node
		//  if ( queue->free_head ) { 
		//  	queue->free_tail->next = head; 	
		//  } else { 
		//  	queue->free_head = head; 
		//  }
		//  // always appended as last cnode - the nodes always keep their ordering 
		//  queue->free_tail = head; 
		//  // node to be recycled 
		//  head->s_i = 0; 
		//  head->size = 0;  
		//  head->next = NULL; 
	} 
	return ret; 
}  

uint32_t gqueue_pop_back( gqueue_obj * queue ) { 
	gqueue_node * tail = queue->tail; 

	if ( tail->size == 0 ) { 
		fprintf(stderr, "gqueue.pop_back: cannot pop from empty queue\n"); 
		exit(-1); 
	} 

	uint32_t ret = tail->elems[tail->s_i + tail->size - 1];  
	tail->size--; 

	if ( tail->size == 0 && tail->prev ) { 
		queue->tail = tail->prev; 	
	  queue->tail->next = NULL;  
		queue->node_ct--; 

		if ( !queue->free_node ) { 	
			queue->free_node = tail; 
		  tail->s_i = 0; 
		  tail->size = 0;  
		 	tail->next = NULL; 
			tail->prev = NULL; 
		} else { 
			free(tail); 
		} 
	} 
	return ret; 
} 

uint32_t gqueue_poll( gqueue_node * head) { 
	//gqueue_node * head = *head_p; 
	if ( head->size == 0 ) { 
		fprintf(stderr, "gqueue.poll: cannot poll empty queue\n"); 
		exit(-1); 
	} 
	return head->elems[head->s_i];   
}  

void gqueue_push(gqueue_obj * queue, uint32_t value){ 
	//gqueue_obj * queue = *queue_i; 	
	// traverse to end of queue 
	gqueue_node * cnode = queue->tail; 
	assert(!cnode->next);

	if ( (cnode->s_i + cnode->size) == (GQUEUE_BLOCK_SZ ) ) { 
		// need to alloc a new block (or use the free one)
		gqueue_node * new_b; 
		// check for freenode
		if ( queue->free_node ) { 
			new_b = queue->free_node; 
			queue->free_node = NULL;  
			//new_b->next = NULL; 
		} else { 
			//need to alloc new block  
			new_b = calloc(sizeof(gqueue_node), 1); 
		}

		queue->node_ct++; 
		cnode->next = new_b; 	
		new_b->prev = cnode; 
		queue->tail = new_b; 	
		cnode = new_b; 	
	} 

	assert( ( cnode->s_i + cnode->size ) < GQUEUE_BLOCK_SZ ); 
	cnode->elems[cnode->s_i + cnode->size] = value;  
	cnode->size++; 
}   

uint32_t * gqueue_as_array( gqueue_obj * queue, unsigned * ct_wb ) { 
	unsigned elem_ct_u = ( ( queue->node_ct - 1 ) * GQUEUE_BLOCK_SZ ) + queue->tail->size ;  
	if ( !elem_ct_u ) { 
		*ct_wb=0; 
		return NULL; 
	} 
	size_t arr_sz = elem_ct_u * sizeof(uint32_t); 

	//gqueue_node * c_node = queue->head; 		
	//while ( c_node )  { 
	//	arr_sz += c_node->size; 
	//} 

 	//	size_t arr_sz = queue->node_ct * sizeof(gqueue_node) * sizeof(uint32_t);  
	//uint32_t * arr = calloc(arr_sz,1 );  
	uint32_t * arr = malloc(arr_sz); 	
	// traverse queue 
	uint32_t * dest = arr; 
	gqueue_node * c_node = queue->head; 
	while (c_node && elem_ct_u--) { 
		// copy  from s_i to s_i + size  
		memcpy(dest, c_node->elems+c_node->s_i, c_node->size * sizeof(uint32_t) ); 
		dest += c_node->size; 
		c_node=c_node->next; 
	} 

	assert ( !c_node && "Elem undercount"); 

	unsigned elem_ct = dest - arr; 
	//printf("elem_
	//assert(elem_ct <= arr_ct); 

	*ct_wb=elem_ct;  
	return arr; 
}  
