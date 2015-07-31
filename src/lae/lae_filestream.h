#ifndef __lae__filestream__
#define __lae__filestream__

#include <lae/lae_stream.h>
#include <lae/lae_string.h>

#include <lae/lae_allocator.h>

typedef enum {
    lae_filestream_access_readonly,
    lae_filestream_accces_writeonly,
    lae_filestream_access_readwrite,
    lae_filesteram_access_append,
} lae_filestream_access;

lae_stream * lae_filestream_make    ( lae_allocator * allocator, lae_string * filename, const lae_filestream_access access );
lae_stream * lae_filestream_create  ( lae_allocator * allocator, lae_string * filename, const lae_filestream_access access );
void lae_filestream_release         ( lae_stream * stream );

lae_stream * lae_stdin   ();
lae_stream * lae_stdout  ();
lae_stream * lae_stderr  ();

#endif
