#include <lae/string.h>

#include <lae/pool.h>

#include <string.h>

typedef size_t      (*lae_string_count_function) ( const lae_string * string );
typedef uint32_t    (*lae_string_at_function)    ( const lae_string * string, const size_t index );
typedef struct {
    lae_string_count_function   count;
    lae_string_at_function      at;
} lae_string_functions;

size_t lae_string_ascii_count( const lae_string * string )
{
    return lae_string_size( string ) - 1;
}

uint32_t lae_string_ascii_at( const lae_string * string, const size_t index )
{
    return lae_string_bytes( string )[index];
}

static lae_string_functions lae_string_functions_null = {};
static lae_string_functions lae_string_functions_ascii = {
    lae_string_ascii_count,
    lae_string_ascii_at
};

struct lae_string {
    lae_allocator *         allocator;
    char *                  bytes;
    size_t                  size;
    lae_string_encodings    encoding;
    lae_string_functions    function;
};

lae_string * lae_string_make( lae_allocator * allocator, const char * bytes, const size_t size, const lae_string_encodings encoding )
{
    lae_string * string = lae_string_create( allocator, bytes, size, encoding );
    lae_pool_add( lae_pool_current(), string, (lae_pool_release_handler)lae_string_release );
    return string;
}

lae_string * lae_string_create( lae_allocator * allocator, const char * bytes, const size_t size, const lae_string_encodings encoding )
{
    lae_string * string = lae_allocator_allocate( allocator, sizeof( lae_string ), 1 );
    if ( string ) {
        string->allocator = allocator;
        string->bytes = (char *)lae_allocator_allocate( allocator, size + 1, 1 );
        memcpy( (void *)string->bytes, bytes, size );
        string->bytes[size] = 0;
        string->size = size;
        string->encoding = encoding;
        switch ( encoding ) {
            case lae_string_encoding_ascii:
                string->function = lae_string_functions_ascii;
                break;
            default:
                string->function = lae_string_functions_null;
                break;
        }
    }
    return string;
}

void lae_string_release( lae_string * string )
{
    lae_allocator_free( string->allocator, (void *)string->bytes );
    lae_allocator_free( string->allocator, string );
}

size_t lae_string_size( const lae_string * string )
{
    return string->size;
}

const char * lae_string_bytes( const lae_string * string )
{
    return string->bytes;
}

lae_string_encodings lae_string_encoding( const lae_string * string )
{
    return string->encoding;
}

size_t lae_string_count( const lae_string * string )
{
    return string->function.count( string );
}

uint32_t lae_string_at( const lae_string * string, const size_t index )
{
    return string->function.at( string, index );
}
