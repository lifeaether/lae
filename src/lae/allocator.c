#include <lae/allocator.h>

#include <pthread.h>
#include <stdlib.h>

struct lae_allocator {
    lae_allocator_allocate_function allocate_function;
    lae_allocator_reallocate_function reallocate_function;
    lae_allocator_free_function free_function;
};

lae_allocator * lae_allocator_create(
                                     const lae_allocator_allocate_function allocate_function,
                                     const lae_allocator_reallocate_function reallocate_function,
                                     const lae_allocator_free_function free_function )
{
    lae_allocator *allocator = allocate_function( sizeof( lae_allocator ), 1 );
    if ( allocator ) {
        allocator->allocate_function = allocate_function;
        allocator->reallocate_function = reallocate_function;
        allocator->free_function = free_function;
    }
    return allocator;
}

void lae_allocator_release( lae_allocator * allocator )
{
    allocator->free_function( allocator );
}

static void * default_allocate( const int32_t size, const int32_t count )
{
    return malloc( size * count );
}

static void * default_reallocate( void * allocated, const int32_t size, const int32_t count )
{
    return realloc( allocated, size * count );
}

static void default_free( void * allocated )
{
    free( allocated );
}

static lae_allocator * default_allocator = NULL;
static void lae_allocator_default_initialize()
{
    default_allocator = lae_allocator_create( default_allocate, default_reallocate, default_free );
}

lae_allocator * lae_allocator_default()
{
    static pthread_once_t once = PTHREAD_ONCE_INIT;
    pthread_once( &once, lae_allocator_default_initialize );
    return default_allocator;
}

void * lae_allocator_allocate( const lae_allocator * allocator, const int32_t size, const int32_t count )
{
    return allocator->allocate_function( size, count );
}

void * lae_allocator_reallocate( const lae_allocator * allocator, void * allocated, const int32_t size, const int32_t count )
{
    return allocator->reallocate_function( allocated, size, count );
}

void lae_allocator_free( const lae_allocator * allocator, void * allocated )
{
    allocator->free_function( allocated );
}
