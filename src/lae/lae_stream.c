#include <lae/lae_stream.h>

#include <lae/lae_pool.h>

struct lae_stream {
    lae_allocator *     allocator;
    void *              info;
    lae_stream_functions function;
};

lae_stream * lae_stream_make( lae_allocator * allocator, void * info, const lae_stream_functions function )
{
    lae_stream * stream = lae_stream_create( allocator, info, function );
    lae_pool_add( lae_pool_current(), stream, (lae_pool_release_handler)lae_stream_release );
    return stream;
}

lae_stream * lae_stream_create( lae_allocator * allocator, void * info, const lae_stream_functions function )
{
    lae_stream * stream = lae_allocator_allocate( allocator, sizeof( lae_stream ), 1 );
    if ( stream ) {
        stream->allocator = allocator;
        stream->function = function;
    }
    return stream;
}

void lae_stream_release( lae_stream * stream )
{
    if ( lae_stream_closable( stream ) ) {
        stream->function.close( stream->info );
    }
    lae_allocator_free( stream->allocator, stream );
}

bool lae_stream_readable( const lae_stream * stream )
{
    return stream->function.read != NULL;
}

lae_stream_result lae_stream_read( lae_stream * stream, void * bytes, const size_t size, const size_t count )
{
    return stream->function.read( stream->info, bytes, size, count );
}

bool lae_stream_writable( const lae_stream * stream )
{
    return stream->function.write != NULL;
}

lae_stream_result lae_stream_write( lae_stream * stream, const void * bytes, const size_t size, const size_t count )
{
    return stream->function.write( stream->info, bytes, size, count );
}

bool lae_stream_seekable( const lae_stream * stream )
{
    return stream->function.offset && stream->function.seek && stream->function.seek_begin && stream->function.seek_end;
}

lae_stream_result lae_stream_offset( lae_stream * stream )
{
    return stream->function.offset( stream->info );
}

lae_stream_result lae_stream_seek( lae_stream * stream, const int64_t offset )
{
    return stream->function.seek( stream->info, offset );
}

lae_stream_result lae_stream_seek_begin( lae_stream * stream )
{
    return stream->function.seek_begin( stream->info );
}

lae_stream_result lae_stream_seek_end( lae_stream * stream )
{
    return stream->function.seek_end( stream->info );
}

bool lae_stream_closable( lae_stream * stream )
{
    return stream->function.close != NULL;
}

lae_stream_result lae_stream_close( lae_stream * stream )
{
    lae_stream_result r = stream->function.close( stream->info );
    if ( r.error == lae_stream_code_ok ) {
        stream->function.close = NULL;
    }
    return r;
}
