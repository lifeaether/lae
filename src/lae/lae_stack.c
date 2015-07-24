
#include <lae/lae_stack.h>

lae_stack * lae_stack_make( lae_allocator * allocator )
{
    return lae_array_make( allocator );
}

lae_stack * lae_stack_create( lae_allocator * allocator )
{
    return lae_array_create( allocator );
}

void lae_stack_release( lae_stack * stack )
{
    lae_array_release( stack );
}

void lae_stack_push( lae_stack * stack, const void * element )
{
    lae_array_add( stack, element );
}

const void * lae_stack_pop( lae_stack * stack )
{
    const int32_t end = lae_array_count( stack )-1;
    const void * element = lae_array_at( stack, end );
    lae_array_remove( stack, end );
    return element;
}

const void * lae_stack_top  ( const lae_stack * stack )
{
    const int32_t index = lae_array_count( stack ) - 1;
    if ( index < 0 ) {
        return NULL;
    } else {
        return lae_array_at( stack, index );
    }
}

const bool lae_stack_empty( const lae_stack * stack )
{
    return lae_array_count( stack ) == 0;
}
