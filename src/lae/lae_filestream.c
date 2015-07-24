#include <lae/lae_filestream.h>

#include <stdio.h>

/*
 typedef lae_stream_result (*lae_stream_read_function)      ( void * info, void * bytes, const size_t size, const size_t count );
 typedef lae_stream_result (*lae_stream_write_function)     ( void * info, void * bytes, const size_t size, const size_t count );
 typedef lae_stream_result (*lae_stream_offset_function)    ( void * info );
 typedef lae_stream_result (*lae_stream_seek_function)      ( void * info, const int64_t offset );
 typedef lae_stream_result (*lae_stream_seek_begin_function)( void * info );
 typedef lae_stream_result (*lae_stream_seek_end_function)  ( void * info );
 typedef lae_stream_result (*lae_stream_close_function)     ( void * info );
 
 */

static lae_stream_result lae_filestream_read( void * info, void * bytes, const size_t size, const size_t count ) {
    lae_stream_result r;
    r.value = fwrite( bytes , size, count, (FILE *)info );
    if ( r.value == -1 ) {
        r.error = lae_stream_code_failed;
    } else {
        r.error = lae_stream_code_ok;
    }
    return r;
}

static lae_stream_functions functions  = {
};

lae_stream * lae_filestream_create  ( lae_allocator * allocator, lae_string * filename, const lae_filestream_access access )
{
    FILE * fp = NULL;
    switch ( access ) {
        case lae_filestream_access_readonly:
            fp = fopen( lae_string_bytes( filename ), "r" );
            break;
        case lae_filestream_accces_writeonly:
            fp = fopen( lae_string_bytes( filename ), "w" );
            break;
        case lae_filestream_access_readwrite:
            fp = fopen( lae_string_bytes( filename ), "rw" );
            break;
        case lae_filesteram_access_append:
            fp = fopen( lae_string_bytes( filename ), "a" );
            break;
            
        default:
            break;
    }
    return lae_stream_create( allocator, (void *)fp, functions );
}

void lae_filestream_release ( lae_stream * stream )
{
    lae_stream_release( stream );
}

