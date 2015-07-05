
#ifndef __lae__stack__
#define __lae__stack__

#include <lae/array.h>
#include <lae/allocator.h>

typedef lae_array lae_stack;

lae_stack * lae_stack_make      ( lae_allocator * allocator );
lae_stack * lae_stack_create    ( lae_allocator * allocator );
void        lae_stack_release   ( lae_stack * stack );

void            lae_stack_push ( lae_stack * stack, const void * element );
const void *    lae_stack_pop  ( lae_stack * stack );
const void *    lae_stack_top  ( const lae_stack * stack );
const bool      lae_stack_empty( const lae_stack * stack );

#endif
