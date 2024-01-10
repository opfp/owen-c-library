#include "gqueue.h" 

gqueue_obj * gqueue_init() { 
	gqueue_obj * new_queue = calloc(sizeof(gqueue_obj) + sizeof(gqueue_node),1); 	
	gqueue_node * new_node = (gqueue_node *) (new_queue+1); 	
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

	//while ( cnode->next ) { cnode = cnode->next; }  
	if ( cnode->size == (GQUEUE_BLOCK_SZ ) ) { //need to alloc new block  
		gqueue_node * new_b = calloc(sizeof(gqueue_node), 1); 
		cnode->next = new_b; 	
		cnode = new_b; 	
		queue->tail = new_b; 	
		queue->node_ct++; 
	} 

	cnode->elems[cnode->s_i + cnode->size] = value;  
	cnode->size++; 
}   

uint32_t * gqueue_as_array( gqueue_obj * queue, unsigned * ct_wb ) { 
	size_t arr_sz = queue->node_ct * sizeof(gqueue_node) * sizeof(uint32_t);  
	uint32_t * arr = malloc(arr_sz );  
	// traverse queue 
	gqueue_node * c_node = queue->head; 		
	uint32_t * dest = arr; 
	while (c_node) { 
		// copy  from s_i to s_i + size  
		memcpy(dest, c_node->elems+c_node->s_i, c_node->size * sizeof(uint32_t) ); 
		dest += c_node->size; 
		c_node=c_node->next; 
	} 
	unsigned elem_ct = dest - arr; 
	//printf("elem_
	//assert(elem_ct <= arr_ct); 

	*ct_wb=elem_ct;  
	return arr; 
}  
