
#ifndef __lae__pool__
#define __lae__pool__

#include <lae/lae_allocator.h>

struct lae_pool;
typedef struct lae_pool lae_pool;

lae_pool * lae_pool_current     ();

lae_pool *  lae_pool_create     ( lae_allocator * allocator );
void        lae_pool_release    ( lae_pool * pool );

typedef void (*lae_pool_release_handler)( void * element );
void lae_pool_add   ( lae_pool * pool, void * element, const lae_pool_release_handler handler );
void lae_pool_remove( lae_pool * pool, const void * element );


#endif
