
#ifndef __lae_array__
#define __lae_array__

#include <lae/allocator.h>
#include <lae/range.h>

#include <stdint.h>
#include <stdbool.h>

struct lae_array;
typedef struct lae_array lae_array;

typedef struct {
    int32_t index;
    bool valid;
} lae_optional_index;
lae_optional_index lae_optional_index_make( const int32_t index, const bool valid );

lae_array * lae_array_make      ( lae_allocator * allocator );
lae_array * lae_array_create    ( lae_allocator * allocator );
void lae_array_release          ( lae_array * array );

const void *    lae_array_at        ( const lae_array * array, const int32_t index );
int32_t         lae_array_count     ( const lae_array * array );

typedef bool (*lae_find_predicate)( void * context, const void * element);
lae_optional_index lae_array_find   ( const lae_array * array, const lae_range range, const lae_find_predicate find, void * context );

void    lae_array_insert    ( lae_array * array, const int32_t index, const void * element );
void    lae_array_add       ( lae_array * array, const void * element );
void    lae_array_remove    ( lae_array * array, const int32_t index );

#endif
