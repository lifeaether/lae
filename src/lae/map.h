
#ifndef __lae__map__
#define __lae__map__

#include <lae/allocator.h>

#include <stdint.h>
#include <stdbool.h>

struct lae_map;
typedef struct lae_map lae_map;
typedef bool (*lae_map_less)( const void * k1, const void * k2 );

lae_map *   lae_map_make    ( lae_allocator * allocator, const lae_map_less less );
lae_map *   lae_map_create  ( lae_allocator * allocator, const lae_map_less less );
void        lae_map_release ( lae_map * map );

const void *    lae_map_get ( const lae_map * map, const void * key );

typedef void (*lae_map_apply_function)( void * context, const void * key, const void * value );
void        lae_map_apply   ( const lae_map * map, const lae_map_apply_function f, void * context );

void    lae_map_set     ( lae_map * map, const void * key, const void * value );
void    lae_map_unset   ( lae_map * map, const void * key );

#endif
