#include <lae/lae_filestream.h>

#include <lae/lae_pool.h>

#include <pthread.h>
#include <stdio.h>
#include <assert.h>

static lae_stream_result lae_filestream_read( void * info, void * bytes, const size_t size, const size_t count ) {
    lae_stream_result r;
    r.value = fread( bytes , size, count, (FILE *)info );
    if ( r.value == -1 ) {
        if ( ferror( (FILE *)info ) ) {
            r.error = lae_stream_code_failed;
        } else if ( feof( (FILE *)info ) ) {
            r.error = lae_stream_code_eof;
        } else {
            r.error = lae_stream_code_undefined;
        }
    } else {
        r.error = lae_stream_code_ok;
    }
    return r;
}

static lae_stream_result lae_filestream_write( void * info, const void * bytes, const size_t size, const size_t count )
{
    lae_stream_result r;
    r.value = fwrite( bytes, size, count, (FILE *)info );
    if ( r.value == -1 ) {
        if ( ferror( (FILE *)info ) ) {
            r.error = lae_stream_code_failed;
        } else if ( feof( (FILE *)info ) ) {
            r.error = lae_stream_code_eof;
        } else {
            r.error = lae_stream_code_undefined;
        }
    } else {
        r.error = lae_stream_code_ok;
    }
    return r;
}

static lae_stream_result lae_filestream_close( void * info )
{
    lae_stream_result r;
    if ( fflush( (FILE *)info ) == 0 && fclose( (FILE *)info ) == 0 ) {
        r.value = 0;
        r.error = lae_stream_code_ok;
    } else {
        r.value = 0;
        r.error = lae_stream_code_failed;
    }
    return r;
}

lae_stream * lae_filestream_make    ( lae_allocator * allocator, lae_string * filename, const lae_filestream_access access )
{
    lae_stream * stream = lae_filestream_create( allocator, filename, access );
    if ( stream ) {
        lae_pool_add( lae_pool_current(), stream, (lae_pool_release_handler)lae_filestream_release );
    }
    return stream;
}

lae_stream * lae_filestream_create  ( lae_allocator * allocator, lae_string * filename, const lae_filestream_access access )
{
    FILE * fp = NULL;
    lae_stream_functions functions = {};
    switch ( access ) {
        case lae_filestream_access_readonly:
            fp = fopen( lae_string_bytes( filename ), "r" );
            functions.read = lae_filestream_read;
            functions.close = lae_filestream_close;
            break;
        case lae_filestream_accces_writeonly:
            fp = fopen( lae_string_bytes( filename ), "r+" );
            functions.write = lae_filestream_write;
            functions.close = lae_filestream_close;
            break;
        case lae_filestream_access_readwrite:
            fp = fopen( lae_string_bytes( filename ), "r+" );
            functions.read = lae_filestream_read;
            functions.write = lae_filestream_write;
            functions.close = lae_filestream_close;
            break;
        case lae_filesteram_access_append:
            fp = fopen( lae_string_bytes( filename ), "r+" );
            functions.read = lae_filestream_read;
            functions.write = lae_filestream_write;
            functions.close = lae_filestream_close;
            fseek( fp, 0, SEEK_END );
            break;
        default:
            assert( 0 );
            break;
    }
    return lae_stream_create( allocator, (void *)fp, functions );
}

void lae_filestream_release ( lae_stream * stream )
{
    lae_stream_release( stream );
}

static lae_stream * filestream_stdin = NULL;
static void lae_filestream_stdin_init()
{
    lae_stream_functions functions = {};
    functions.read = lae_filestream_read;
    filestream_stdin = lae_stream_create( lae_allocator_default(), (void *)stdin, functions );
}

lae_stream * lae_filestream_stdin()
{
    static pthread_once_t once = PTHREAD_ONCE_INIT;
    pthread_once( &once, lae_filestream_stdin_init );
    return filestream_stdin;
}

static lae_stream * filestream_stdout = NULL;
static void lae_filestream_stdout_init()
{
    lae_stream_functions functions = {};
    functions.write = lae_filestream_write;
    filestream_stdout = lae_stream_create( lae_allocator_default(), (void *)stdout, functions );
}

lae_stream * lae_filestream_stdout()
{
    static pthread_once_t once = PTHREAD_ONCE_INIT;
    pthread_once( &once, lae_filestream_stdout_init );
    return filestream_stdout;
}

static lae_stream * filestream_stderr = NULL;
static void lae_filestream_stderr_init()
{
    lae_stream_functions functions = {};
    functions.write = lae_filestream_write;
    filestream_stderr = lae_stream_create( lae_allocator_default(), (void *)stderr, functions );
}

lae_stream * lae_filestream_stderr()
{
    static pthread_once_t once = PTHREAD_ONCE_INIT;
    pthread_once( &once, lae_filestream_stderr_init );
    return filestream_stderr;
}
