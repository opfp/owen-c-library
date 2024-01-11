#include <stdio.h> 
#include "../rst.h" 

#define STRS_L_C 8 

char * test_strs_l[STRS_L_C] = { 
	"albany", 
	"albert",  
	"albatros", 
	"guide", 
	"guise", 
	"gutenburg", 
	"wisconsin", 
	"wish" 
}; 

char * fail_strs_l[STRS_L_C] = { 
	"algae", 
	"alboot",  
	"broom", 
	"goth", 
	"gud", 
	"gutentag", 
	"wiscorn", 
	"zebra" 
};  

char fail_strs_l_i[STRS_L_C] = { 
	2, 
	3, 
	0, 
	1, 
	2, 
	5, 
	5, 	
	0 
};  

#define STR_S_C 9 

char * test_strs_s[STR_S_C] = { 
	"abc", 
	"abd", 
	"arm", 
	"col",  
	"cor", 
	"wwe", 
	"www",
	"ezz", 
	"zzz"
}; 

char * fail_queries_s[] = { 
	"abl", 
	"aws", 
	"bi", 
	"cod", 
	"wwf" 
}; 

char fail_queries_s_i[] = { 
	2, 
	1, 
	0, 	
	2, 
	2 
}; 

void bit_test() { 
	char * buffer = calloc(4,1); 	
	SET_BIT(buffer, 27); 
	//SET_BIT(buffer, 11); 
	//printf("%hhX\n", *(buffer+1) );  

	assert(CHECK_BIT(buffer, 27)); 

} 


//void bm_rst_test() { 
//	bitmap_rst * rst = brst_init(3);  
//
//	for ( int i = 0; i < STR_S_C; ++i ) { 
//		brst_in(rst, test_strs_s[i] ); 	
//	} 
//
//	brst_pos begin_pos = (brst_pos) { 0, 0, 0, NULL };  
//	brst_pos pos; 
//	
//	for ( int i = 0; i < STR_S_C; ++i ) { 
//		pos = begin_pos; 
//		for ( int j = 0; j < 3; ++j ) { 
//			if ( ! brst_advance( rst, &pos, test_strs_s[i][j] ) ) { 
//				printf("test fail, %s at %i (%c)\n", test_strs_s[i], j, test_strs_s[i][j] );  
//				j=3; 
//			}  
//		} 
//	} 
//
//	char ex_fail, brst_ret; 
//
//	for ( int i = 0; i < 5; ++i ) { 
//		pos = begin_pos; 
//		ex_fail = fail_queries_s_i[i];  
//
//		for ( char j = 0; j < 3; ++j ) { 
//			brst_ret = brst_advance( rst, &pos, fail_queries_s[i][j] ); 
//			if ( !brst_ret && j < ex_fail ) { 			
//				printf("fail where advance expected, %s at %i\n",  fail_queries_s[i], j );  
//				j=3; 
//			}  
//			if ( brst_ret && j == ex_fail ) { 
//				printf("advance where fail expected, %s at %i\n", fail_queries_s[i], j ); 
//				j=3; 
//			} 	 
//
//			if ( ! brst_ret ) 
//				j=3; 
//		} 
//	} 
//} 

void h_rst_test() { 
	bitmap_rst * rst = brst_init(3); 

	for ( short i = 0; i < STRS_L_C; ++i ) { 
		brst_in(rst, test_strs_l[i], i); 	
	} 

	brst_pos begin_pos = (brst_pos) { 0, 0, 0, NULL, 0 };  
	brst_pos pos; 
	int j, rst_ret; 
	char c, * sp; 
	ll_rst_node * cnode; 

	//	for ( short i = 0; i < STRS_L_C; ++i ) {
	//		pos = begin_pos; 
	//		sp = test_strs_l[i]; 	
	//		j=0; 
	//		while ( ( c = *sp++ ) ) { 
	//			rst_ret = brst_advance(rst, &pos, c ); 
	//			if ( rst_ret == BRST_NOWORD  ) { 
	//				printf("test fail, %s at %i (%c)\n", test_strs_l[i], j, test_strs_l[i][j] );	
	//				goto _cont_1; 
	//			} else if ( rst_ret == BRST_EOW  ) { 
	//				//printf("EOW for %s (%i)\n", test_strs_l[i], pos.sid );  
	//				if ( *sp ) { 
	//					printf("EOW recieved before end of test string %i\n", i ); 
	//					goto _cont_1; 
	//				}  
	//				if ( pos.sid != i ) { 
	//					printf("Wrong sid returned for test string %i\n", i );
	//					goto _cont_1; 
	//				}  
	//			}  	
	//			j++; 	
	//		} 
	//		_cont_1:; 
	//	}  

	int ex_now; 	

	for ( short i = 0; i < STRS_L_C; ++i ) {
		pos = begin_pos; 
		sp = fail_strs_l[i]; 	
		j=0; 
		ex_now = (int) fail_strs_l_i[i]; 
		while ( ( c = *sp++ ) ) { 
			rst_ret = brst_advance(rst, &pos, c ); 
			if ( rst_ret == BRST_WORD && j < ex_now ) 
				goto _inner_cont;  
			if ( rst_ret == BRST_NOWORD && j == ex_now ) 
				goto _cont_2; 

			printf("Unexpected state, ret: %i, i: %i, j: %i\n", rst_ret, i, j ); 
			goto _cont_2;  

			_inner_cont:; 
			++j; 
		}
		_cont_2:; 
	} 
	//	for ( int i = 0; i < STRS_L_C; ++i ) { 
	//		pos = begin_pos; 
	//		for ( j = 0; j < 3; ++j ) { 
	//			if ( ! brst_advance(rst, &pos, test_strs_l[i][j] ) ) { 
	//				printf("test fail, %s at %i (%c)\n", test_strs_l[i], j, test_strs_l[i][j] );  
	//				goto _cont ; 
	//			} 
	//		} 
	//
	//		brst_advance(rst, &pos, test_strs_l[i][j] ); 
	//
	//		if ( ! ( cnode = pos.ll_node ) ) { 
	//			printf("ll_node null where valid node expected, %s at %i\n", test_strs_l[i], j );   
	//			goto _cont; 
	//		}  
	//
	//		while ( ( c = test_strs_l[i][j] ) ) { 
	//			if ( ! (cnode = (ll_rst_node *) cnode->children[c-'a']) ) { 
	//				printf("ll_node child (%c) expected in %s at %i\n", c, test_strs_l[i], j ); 
	//				goto _cont; 
	//			}  
	//			++j; 
	//		}  
	//
	//		_cont:;  
	//	} 
} 

int main() { 
	//bit_test(); 
	//bm_rst_test(); 
	h_rst_test(); 
} 
