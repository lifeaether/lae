
#include <lae/lae_pool.h>

#include <lae/lae_stack.h>
#include <lae/lae_array.h>
#include <lae/lae_map.h>

#include <pthread.h>

struct lae_pool {
    lae_allocator * allocator;
    lae_map * elements;
};

typedef struct lae_pair_thread_stack {
    pthread_t thread;
    lae_stack * stack;
    
} lae_pair_thread_stack;

static lae_pair_thread_stack * lae_pair_thread_stack_create( lae_allocator * allocator, const pthread_t thread, lae_stack * stack )
{
    lae_pair_thread_stack *pair = lae_allocator_allocate( allocator, sizeof( lae_pair_thread_stack), 1 );
    if ( pair ) {
        pair->thread = thread;
        pair->stack = stack;
    }
    return pair;
}

static void lae_pair_thread_stack_release( lae_allocator * allocator, lae_pair_thread_stack * pair )
{
    lae_allocator_free( allocator, pair );
}

static lae_array * thread_pools = NULL;
static void thread_pools_initialize()
{
    thread_pools = lae_array_create( lae_allocator_default() );
}
static lae_array * thread_pools_get()
{
    static pthread_once_t once = PTHREAD_ONCE_INIT;
    pthread_once( &once, thread_pools_initialize );
    return thread_pools;
}

static bool lae_thread_pool_find( pthread_t * thread, lae_pair_thread_stack * element )
{
    return pthread_equal( *thread, element->thread );
}

lae_stack * lae_pool_current_stack()
{
    lae_array * thread_pools = thread_pools_get();
    
    pthread_t thread = pthread_self();
    const lae_range range = lae_range_make( 0, lae_array_count( thread_pools ) );
    const lae_optional_index index = lae_array_find( thread_pools, range, (lae_find_predicate)lae_thread_pool_find, &thread );
    if ( index.valid ) {
        const lae_pair_thread_stack * thread_stack = lae_array_at( thread_pools, index.index );
        return thread_stack->stack;
    } else {
        lae_stack * stack = lae_stack_create( lae_allocator_default() );
        lae_pair_thread_stack * thread_stack = lae_pair_thread_stack_create( lae_allocator_default(), pthread_self(), stack );
        lae_array_add( thread_pools, thread_stack );
        return stack;
    }
}

lae_pool * lae_pool_current()
{
    lae_stack * stack = lae_pool_current_stack();
    if ( lae_stack_empty( stack ) ) {
        return NULL;
    } else {
        return (lae_pool *)lae_stack_top( stack );
    }
}

bool element_less( const void * e1, const void * e2 )
{
    return e1 < e2;
}

lae_pool * lae_pool_create( lae_allocator * allocator )
{
    lae_pool * pool = lae_allocator_allocate( allocator, sizeof( lae_pool ), 1 );
    if ( pool ) {
        pool->allocator = allocator;
        pool->elements = lae_map_create( allocator, element_less );
        lae_stack_push( lae_pool_current_stack(), pool );
    }
    return pool;
}

static void lae_pool_release_apply( void * context, void * element, const lae_pool_release_handler handler )
{
    handler( element );
}

void lae_pool_release( lae_pool * pool )
{
    lae_stack_pop( lae_pool_current_stack() );
    
    lae_map_apply( pool->elements, (lae_map_apply_function)lae_pool_release_apply, NULL );
    lae_map_release( pool->elements );
    lae_allocator_free( pool->allocator, pool );
}

void lae_pool_add   ( lae_pool * pool, void * element, const lae_pool_release_handler handler )
{
    lae_map_set( pool->elements, element, handler );
}

void lae_pool_remove( lae_pool * pool, const void * element )
{
    lae_map_unset( pool->elements, element );
}

