#ifndef __lae_stream_h
#define __lae_stream_h

#include <lae/lae_allocator.h>

#include <stdint.h>
#include <stdbool.h>

struct lae_stream;
typedef struct lae_stream lae_stream;

typedef enum {
    lae_stream_code_ok                  = 0,
    lae_stream_code_failed              = 1,
    lae_stream_code_failed_with_offset  = 2,
    lae_stream_code_undefined           = 9,
    lae_stream_code_eof                 = 100,
    lae_stream_code_last                = 10000,

    lae_stream_code_user    = 20000,
} lae_stream_codes;

typedef struct {
    size_t              value;
    lae_stream_codes    error;
} lae_stream_result;
lae_stream_result lae_stream_result_make    ( const size_t value, const lae_stream_codes code );

typedef lae_stream_result (*lae_stream_read_function)      ( void * info, void * bytes, const size_t size, const size_t count );
typedef lae_stream_result (*lae_stream_write_function)     ( void * info, const void * bytes, const size_t size, const size_t count );
typedef lae_stream_result (*lae_stream_offset_function)    ( void * info );
typedef lae_stream_result (*lae_stream_seek_function)      ( void * info, const int64_t offset );
typedef lae_stream_result (*lae_stream_seek_begin_function)( void * info );
typedef lae_stream_result (*lae_stream_seek_end_function)  ( void * info );
typedef lae_stream_result (*lae_stream_close_function)     ( void * info );

typedef struct {
    lae_stream_read_function        read;
    lae_stream_write_function       write;
    lae_stream_offset_function      offset;
    lae_stream_seek_function        seek;
    lae_stream_seek_begin_function  seek_begin;
    lae_stream_seek_end_function    seek_end;
    lae_stream_close_function       close;
} lae_stream_functions;

lae_stream *        lae_stream_make         ( lae_allocator * allocator, void * info, const lae_stream_functions function );
lae_stream *        lae_stream_create       ( lae_allocator * allocator, void * info, const lae_stream_functions function );
void                lae_stream_release      ( lae_stream * stream );

bool                lae_stream_readable     ( const lae_stream * stream );
lae_stream_result   lae_stream_read         ( lae_stream * stream, void * bytes, const size_t size, const size_t count );

bool                lae_stream_writable     ( const lae_stream * stream );
lae_stream_result   lae_stream_write        ( lae_stream * stream, const void * bytes, const size_t size, const size_t count );

bool                lae_stream_seekable     ( const lae_stream * stream );
lae_stream_result   lae_stream_offset       ( lae_stream * stream );
lae_stream_result   lae_stream_seek         ( lae_stream * stream, const int64_t offset );
lae_stream_result   lae_stream_seek_begin   ( lae_stream * stream );
lae_stream_result   lae_stream_seek_end     ( lae_stream * stream );

bool                lae_stream_closable     ( lae_stream * stream );
lae_stream_result   lae_stream_close        ( lae_stream * stream );

#endif
