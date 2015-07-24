
#include <lae/lae_array.h>
#include <lae/lae_pool.h>

#include <string.h>

lae_optional_index lae_optional_index_make( const int32_t index, const bool valid )
{
    lae_optional_index optional_index = { index, valid };
    return optional_index;
}

struct lae_array {
    lae_allocator * allocator;
    const void ** elements;
    int32_t size;
    int32_t capacity;
};

lae_array * lae_array_make( lae_allocator * allocator )
{
    lae_array * array = lae_array_create( allocator );
    lae_pool_add( lae_pool_current(), array, (lae_pool_release_handler)lae_array_release );
    return array;
}

lae_array * lae_array_create( lae_allocator * allocator )
{
    lae_array * array = lae_allocator_allocate( allocator, sizeof( lae_array ), 1 );
    if ( array ) {
        array->allocator = allocator;
        array->capacity = 1;
        array->size = 0;
        array->elements = lae_allocator_allocate( allocator, sizeof( void * ), array->capacity );
    }
    return array;
}

void lae_array_release( lae_array * array )
{
    lae_allocator_free( array->allocator, array->elements );
    lae_allocator_free( array->allocator, array );
}

const void * lae_array_at( const lae_array * array, const int32_t index )
{
    return array->elements[index];
}

int32_t lae_array_count( const lae_array * array )
{
    return array->size;
}

lae_optional_index lae_array_find   ( const lae_array * array, const lae_range range, const lae_find_predicate find, void * context )
{
    for ( int32_t i = lae_range_begin( range ); i < lae_range_end( range ); i++ ) {
        if ( find( context, lae_array_at( array, i ) ) ) {
            return lae_optional_index_make( i, true );
        }
    }
    return lae_optional_index_make( -1, false );
}

void lae_array_insert( lae_array * array, const int32_t index, const void * element )
{
    if ( array->size < array->capacity ) {
        memmove( array->elements + index + 1, array->elements + index, sizeof( void * ) * (array->size - index) );
        array->elements[index] = element;
        ++array->size;
    } else {
        array->capacity ++;
        //array->capacity *= 2;
        array->elements = lae_allocator_reallocate( array->allocator, array->elements, sizeof( void * ), array->capacity );
        lae_array_insert( array, index, element );
    }
}

void lae_array_add( lae_array * array, const void * element )
{
    lae_array_insert( array, lae_array_count( array ), element );
}

void lae_array_remove( lae_array * array, const int32_t index )
{
    --array->size;
    memmove( array->elements + index, array->elements + index + 1, sizeof( void *) * (array->size - index) );
}
