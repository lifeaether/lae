#ifndef __lae_stream_h
#define __lae_stream_h

#include <lae/allocator.h>

#include <stdint.h>
#include <stdbool.h>

struct lae_stream;
typedef struct lae_stream lae_stream;

typedef size_t (*lae_stream_read_function)      ( void * info, void * bytes, size_t size, size_t count );
typedef size_t (*lae_stream_write_function)     ( void * info, void * bytes, size_t size, size_t count );
typedef size_t (*lae_stream_offset_function)    ( void * info );
typedef size_t (*lae_stream_seek_function)      ( void * info, int64_t offset );
typedef size_t (*lae_stream_seek_begin_function)( void * info );
typedef size_t (*lae_stream_seek_end_function)  ( void * info );
typedef size_t (*lae_stream_close_function)     ( void * info );

typedef struct {
    lae_stream_read_function        read;
    lae_stream_write_function       write;
    lae_stream_offset_function      offset;
    lae_stream_seek_function        seek;
    lae_stream_seek_begin_function  seek_begin;
    lae_stream_seek_end_function    seek_end;
    lae_stream_close_function       close;
} lae_stream_function;

lae_stream *    lae_stream_make         ( lae_allocator * allocator, void * info, const lae_stream_function function );
lae_stream *    lae_stream_create       ( lae_allocator * allocator, void * info, const lae_stream_function function );
void            lae_stream_release      ( lae_stream * stream );

bool            lae_stream_readable     ( const lae_stream * stream );
size_t          lae_stream_read         ( lae_stream * stream, void * bytes, size_t size, size_t count );

bool            lae_stream_writable     ( const lae_stream * stream );
size_t          lae_stream_write        ( lae_stream * stream, void * bytes, size_t size, size_t count );

bool            lae_stream_seekable     ( const lae_stream * stream );
size_t          lae_stream_offset       ( lae_stream * stream );
size_t          lae_stream_seek         ( lae_stream * stream, int64_t offset );
size_t          lae_stream_seek_begin   ( lae_stream * stream );
size_t          lae_stream_seek_end     ( lae_stream * stream );

#endif
