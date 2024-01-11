#ifndef O_RST 
#define O_RST 

#define SET_BIT(buf,idx) ( *((buf)+((idx)/8)) |= ( 1 << ((idx)%8) ) )
#define CHECK_BIT(buf,idx) ( *((buf)+((idx)/8)) & ( 1 << ((idx)%8) ) )

#define BRST_NOWORD 0  
#define BRST_WORD 1 
#define BRST_EOW 2 

#include <stdlib.h> 
#include <assert.h> 
#include <stdint.h> 
#include "../clib/htable.h" 

typedef struct { 
	unsigned char depth; 
	char * bit_field; 
	unsigned short bit_field_sz; 
	htable_header * sufx_map; 	
	//char * uni_prefix; 
	unsigned short level_offsets[3]; 
} bitmap_rst; 

typedef struct { 
	short eos; 
	void * children[26]; 
} ll_rst_node; 

typedef struct { 
	char level; 
	unsigned pos; 
	uint32_t c_str; 
	ll_rst_node * ll_node; 
	short sid; 
} brst_pos; 

//typedef struct { 
//	hybrid_rst * rst; 
//	char level; 
//	unsigned short pre_pos; // number of BITS from begining of current level  	
//	//ll_node * sufxs; 
//} hrst_pos; 

bitmap_rst * brst_init(unsigned char depth); 
void brst_in(bitmap_rst * rst, char * instr, short sid);  
int brst_advance( bitmap_rst * rst, brst_pos * pos, char c); 

//hrst_pos hrst_advance(hybrid_rst * rst, hrst_pos pos, char c);  


void lrst_in(ll_rst_node * root, char * instr, short sid);  

#endif 
