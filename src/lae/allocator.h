#ifndef __lae_allocator__
#define __lae_allocator__

#include <stdint.h>

struct lae_allocator;
typedef struct lae_allocator lae_allocator;

typedef void * (*lae_allocator_allocate_function)(const int32_t size, const int32_t count);
typedef void * (*lae_allocator_reallocate_function)(void *, const int32_t size, const int32_t count);
typedef void (*lae_allocator_free_function)(void *);

lae_allocator * lae_allocator_create    (
                                         const lae_allocator_allocate_function allocate_function,
                                         const lae_allocator_reallocate_function reallocate_function,
                                         const lae_allocator_free_function free_function );
void            lae_allocator_release   ( lae_allocator * allocator );

lae_allocator * lae_allocator_default   ();

void *          lae_allocator_allocate      ( const lae_allocator * allocator, const int32_t size, const int32_t count );
void *          lae_allocator_reallocate    ( const lae_allocator * allocator, void * allocated, const int32_t size, const int32_t count );
void            lae_allocator_free          ( const lae_allocator * allocator, void * allocated );

#endif
