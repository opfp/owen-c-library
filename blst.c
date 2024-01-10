#include "blst.h"

block_list * blst_construct() {
    block_list * ret = calloc(1, sizeof(block_list));
    ret->head = calloc(BLOCKSIZE, sizeof(BLST_DTYPE));
    ret->tail = ret->head;
    return ret;
}

void blst_destruct(block_list * this) {
    block * curr = this->head;
    do {
        block * cnxt = curr->next;
        free(curr);
        curr = cnxt;
    } while ( curr );
    free(this);
}

void blst_append(block_list * this, BLST_DTYPE item) {
    assert(this && "Cannot operate on null Block List" );
    unsigned jumps = this->elem_ct / BLOCKSIZE;
    block * curr = this->head;
    while( jumps-- ) {
        curr = curr->next;
    }
    int i = this->elem_ct % BLOCKSIZE;
    if ( i == (BLOCKSIZE - 1) ) { // add block
        curr->next = malloc(sizeof(block));
        ((block *) curr->next)->next = NULL;
    }
    curr->items[i] = item;
    ++this->elem_ct;
}

BLST_DTYPE blst_geti(block_list * this, unsigned geti ){
    assert(this && "Cannot operate on null Block List" );
    assert(geti < this->elem_ct && "Index out of bounds");

    block * curr = this->head;
    unsigned jumps = geti / BLOCKSIZE;
    while ( jumps-- ) {
        curr = curr->next;
    }
    return curr->items[geti % BLOCKSIZE];
}

BLST_DTYPE * blst_as_array( block_list * this ) {
    assert(this && "Cannot operate on null Block List" );
    unsigned ct;
    if( !( ct = this->elem_ct ) ) {
        return NULL;
    }
    BLST_DTYPE * ret = malloc(ct * sizeof(BLST_DTYPE));
    block * curr = this->head;
    int j;
    for ( unsigned i = 0; i < ct; ++i ) {
        j = i % BLOCKSIZE;
        ret[i] = curr->items[j];
        if ( j == (BLOCKSIZE - 1) ) {
            curr = curr->next; //never fails! becase as soon as we write the last elem, we malloc a new even if it's the last one.
        }
    }
    return ret;
}
