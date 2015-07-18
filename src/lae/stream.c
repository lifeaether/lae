#include <lae/stream.h>

#include <lae/pool.h>

struct lae_stream {
    lae_allocator *     allocator;
    void *              info;
    lae_stream_function function;
};

lae_stream * lae_stream_make( lae_allocator * allocator, void * info, const lae_stream_function function )
{
    lae_stream * stream = lae_stream_create( allocator, info, function );
    lae_pool_add( lae_pool_current(), stream, (lae_pool_release_handler)lae_stream_release );
    return stream;
}

lae_stream * lae_stream_create( lae_allocator * allocator, void * info, const lae_stream_function function )
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
    if ( stream->function.close ) {
        stream->function.close( stream->info );
    }
    lae_allocator_free( stream->allocator, stream );
}

bool lae_stream_readable( const lae_stream * stream )
{
    return stream->function.read != NULL;
}

size_t lae_stream_read( lae_stream * stream, void * bytes, size_t size, size_t count )
{
    return stream->function.read( stream->info, bytes, size, count );
}

bool lae_stream_writable( const lae_stream * stream )
{
    return stream->function.write != NULL;
}

size_t lae_stream_write( lae_stream * stream, void * bytes, size_t size, size_t count )
{
    return stream->function.write( stream->info, bytes, size, count );
}

bool lae_stream_seekable( const lae_stream * stream )
{
    return stream->function.offset && stream->function.seek && stream->function.seek_begin && stream->function.seek_end;
}

size_t lae_stream_offset( lae_stream * stream )
{
    return stream->function.offset( stream->info );
}

size_t lae_stream_seek( lae_stream * stream, int64_t offset )
{
    return stream->function.seek( stream->info, offset );
}

size_t lae_stream_seek_begin( lae_stream * stream )
{
    return stream->function.seek_begin( stream->info );
}

size_t lae_stream_seek_end( lae_stream * stream )
{
    return stream->function.seek_end( stream->info );
}