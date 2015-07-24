
#include <lae/lae_map.h>
#include <lae/lae_pool.h>

struct lae_map_node;
typedef struct lae_map_node lae_map_node;

struct lae_map {
    lae_allocator *allocator;
    lae_map_less less;
    lae_map_node * root;
};

struct lae_map_node {
    const void * key;
    const void * value;
    lae_map_node * left;
    lae_map_node * right;
};

static lae_map_node * lae_map_node_create( lae_allocator * allocator, const void * key, const void * value )
{
    lae_map_node * node = lae_allocator_allocate( allocator, sizeof( lae_map_node ), 1 );
    if ( node ) {
        node->key = key;
        node->value = value;
        node->left = NULL;
        node->right = NULL;
    }
    return node;
}

static void lae_map_node_release( lae_map_node * node, lae_allocator * allocator )
{
    lae_allocator_free( allocator, node );
}

typedef void (*lae_map_node_apply_function)( void * context, lae_map_node * node );
static void lae_map_node_apply( lae_map_node * node, const lae_map_node_apply_function f, void * context )
{
    if ( node ) {
        lae_map_node_apply( node->left, f, context );
        lae_map_node_apply( node->right, f, context );
        f( context, node );
    }
}

lae_map * lae_map_make( lae_allocator * allocator, const lae_map_less less )
{
    lae_map * map = lae_map_create( allocator, less );
    lae_pool_add( lae_pool_current(), map, (lae_pool_release_handler)lae_map_release );
    return map;
}

lae_map * lae_map_create( lae_allocator * allocator, const lae_map_less less )
{
    lae_map * map = lae_allocator_allocate( allocator, sizeof( lae_map ), 1 );
    if ( map ) {
        map->allocator = allocator;
        map->less = less;
        map->root = NULL;
    }
    return map;
}

static void lae_map_release_node( lae_allocator * allocator, lae_map_node * node )
{
    lae_map_node_release( node, allocator );
}

void lae_map_release( lae_map * map )
{
    lae_map_node_apply( map->root, (lae_map_node_apply_function)lae_map_release_node, map->allocator );
    lae_allocator_free( map->allocator, map );
}

const void * lae_map_get( const lae_map * map, const void * key )
{
    lae_map_node * node = map->root;
    while ( node ) {
        if ( map->less( node->key, key ) ) {
            node = node->left;
        } else if ( map->less( key, node->key ) ) {
            node = node->right;
        } else {
            return node->value;
        }
    }
    return NULL;
}


typedef struct {
    lae_map_apply_function f;
    void * context;
} lae_map_apply_node_context;

static void lae_map_apply_node( lae_map_apply_node_context * c, lae_map_node *node )
{
    c->f( c->context, node->key, node->value );
}

void lae_map_apply( const lae_map * map, const lae_map_apply_function f, void * context )
{
    lae_map_apply_node_context c = { f, context };
    lae_map_node_apply( map->root, (lae_map_node_apply_function)lae_map_apply_node, &c );
}

void lae_map_set( lae_map * map, const void * key, const void * value )
{
    if ( map->root ) {
        lae_map_node * node = map->root;
        for ( ; ; ) {
            if ( map->less( node->key, key ) ) {
                if ( node->left ) {
                    node = node->left;
                } else {
                    node->left = lae_map_node_create( map->allocator, key, value );
                    break;
                }
            } else if ( map->less( key, node->key ) ) {
                if ( node->right ) {
                    node = node->right;
                } else {
                    node->right = lae_map_node_create( map->allocator, key, value );
                    break;
                }
            } else {
                node->value = value;
                break;
            }
        }
    } else {
        map->root = lae_map_node_create( map->allocator, key, value );
    }
}

void lae_map_unset( lae_map * map, const void * key )
{
    lae_map_node ** replace = NULL;
    lae_map_node * node = map->root;
    while ( node ) {
        if ( map->less( node->key, key ) ) {
            if ( node->left ) {
                replace = &node->left;
                node = node->left;
            } else {
                break;
            }
        } else if ( map->less( key, node->key ) ) {
            if ( node->right ) {
                replace = &node->right;
                node = node->right;
            } else {
                break;
            }
        } else {
            lae_map_node * it = node;
            if ( it->left && it->right ) {
                node = it->left;
                while ( node->right ) node = node->right;
                *replace = node;
            } else if ( it->left ) {
                *replace = it->left;
            } else if ( it->right ) {
                *replace = it->right;
            } else {
                *replace = NULL;
            }
            lae_map_node_release( it, map->allocator );
            break;
        }
    }
}

