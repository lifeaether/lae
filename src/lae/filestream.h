#ifndef __lae__filestream__
#define __lae__filestream__

#include <lae/stream.h>

#include <lae/allocator.h>

lae_stream * lae_filestream_create  ( lae_allocator * allocator, const char * filename );

#endif
