#include "rst.h" 

//inline set_bit( char * buf, unsigned idx ) { 
//	buf+=idx/8; 
//	char map = 1 << ( idx%8 ); 
//	*buf |= map; 	
//}  

bitmap_rst * brst_init( unsigned char depth /*, char has_children*/ ) { 
	bitmap_rst * ret = calloc(sizeof(bitmap_rst), 1); 
	assert ( depth < 4 ); 
	ret->depth = depth; 
	// need sum from of 26^(1 to depth) bits for rst bitmap  
	unsigned short bit_ct = 26; 
	unsigned short c_mult = 26; 
	for ( int i = 1; i < depth; ++i ) {  
		ret->level_offsets[i-1] = bit_ct; 	
		c_mult *= 26; 
		bit_ct += c_mult; 
	} 
	
	ret->bit_field_sz = bit_ct; 
	ret->bit_field = calloc( (bit_ct/8)+1 , 1); 

	ret->sufx_map = htable_init(10); 

	return ret; 
} 

//bitmap_rst * htst_init_cp( bitmap_rst * template,  

short brst_in( bitmap_rst * rst, char * instr, short sid ) { 
	char * rv = instr; 	
	char l = 0; 
	unsigned char l_s = rst->depth; 
	unsigned char c; 
	unsigned cbit = 0; 
	unsigned c_off = 0; 
	unsigned char c_l = 0; 
	uint32_t c_str = 0; 

	while ( l < l_s ) { 
		c = *rv++; 
		if ( !c ) 
			assert(0);  
		assert( c >= 'a' && c <= 'z' );  
		cbit += ( c - 'a' ); 
		SET_BIT(rst->bit_field, (cbit+c_off) ); 	

		// the key for suffix htable	
		c_str <<= 4;  
		c_str |= (uint32_t) ( c - 'a' );  

		cbit *= 26; 
		c_off = rst->level_offsets[l]; 
		++l; 
	}  

	// we've filled the bitmap portion of the rst, now into the ll portion

	ll_rst_node * cnode = (ll_rst_node *) htable_get(rst->sufx_map, c_str ); 
	if ( cnode == NULL ) { 
		cnode = calloc(sizeof(ll_rst_node), 1);  
		htable_set(rst->sufx_map, c_str, cnode); 
	}  

	return lrst_in( cnode, rv, sid ); 
} 

int brst_advance( bitmap_rst * rst, brst_pos * pos, char c) { 
	assert ( 'a' <= c && 'z' >= c );  
	char level = pos->level; 
	assert ( level >= 0 ); 
	//assert ( level >= 0 && level <= rst->depth ); 

	_ll_lookup:; 

	if ( level > rst->depth ) { // ll_advance  
		pos->ll_node = pos->ll_node->children[c-'a']; 
		if ( ! pos->ll_node ) { 
			pos->level=-1; 
			return BRST_NOWORD; 
		} 
		short sid;  
		if ( ( sid = pos->ll_node->eos ) ) { 
			pos->sid = sid;  
			return BRST_EOW; 
		} 

		return BRST_WORD; 
	} 

	if ( level == rst->depth ) { // transition from bitmap mode to ll mode 
		ll_rst_node * cnode = (ll_rst_node *) htable_get(rst->sufx_map, pos->c_str ); 
		if ( cnode == NULL ) {  
			pos->level=-1; 
			return BRST_NOWORD; 
		} 

		pos->level++; 	
		pos->ll_node = cnode; 

		short sid;  
		if ( ( sid = cnode->eos ) ) { 
			pos->sid = sid;  
			return BRST_EOW; 
		} 

		//return BRST_WORD; 
		level++; 
		goto _ll_lookup;  
	}  		
	// bitmap mode 	
	pos->c_str <<= 4;  
	pos->c_str |= (uint32_t) ( c - 'a' );  

	unsigned plo = level < 2 ? 0 : rst->level_offsets[level-2] ; 
	unsigned nlo = level == 0 ? 0 : rst->level_offsets[level-1] ; 

	unsigned n_pos = ( ( pos->pos - plo ) * 26 ) + nlo + ( c - 'a' );  

	if ( CHECK_BIT(rst->bit_field, n_pos ) ) { 
		pos->level++; 
		pos->pos = n_pos; 
		return BRST_WORD; 
	} 

	pos->level = -1; 
	return BRST_NOWORD; 
} 

short lrst_in( ll_rst_node * root, char * instr, short sid) { 
	//ll_rst_node * cnode = root; 
	ll_rst_node * nnode; 
	int idx; 
	char c; 
	while ( (c = *instr++) ) { 
		idx = c-'a'; 
		assert( -1 < idx && idx < 32 ); 
		nnode = root->children[idx]; 
		if ( nnode == NULL ) { 
			nnode = calloc(sizeof(ll_rst_node),1); 	
			root->children[idx] = nnode; 
		} 
		root = nnode; 	
	} 

	if ( root->eos ) { 
		return root->eos; 
	} 

	root->eos = sid; 
	return 0; 
}  

//lrst_advance( ll_rst_node * node, char c) { 
//	
//} 
