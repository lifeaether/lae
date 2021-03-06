//
//  main.m
//  test
//
//  Created by lifeaether on 2015/06/20.
//  Copyright (c) 2015年 lifeaether. All rights reserved.
//

#include <lae/lae.h>

#include <stdio.h>
#include <string.h>

static bool string_less( const char *s1, const char *s2 ) {
    return strcmp( s1, s2 ) < 0;
}

int main(int argc, const char * argv[]) {
    lae_pool *pool = lae_pool_create( lae_allocator_default() );

    {
        lae_array * array = lae_array_make( lae_allocator_default() );

        lae_array_insert( array, 0, "zero" );
        lae_array_insert( array, 0, "one" );
        lae_array_insert( array, 0, "two" );
        lae_array_insert( array, 0, "three" );
        lae_array_insert( array, 0, "four" );
        lae_array_remove( array, 4 );
        for ( int32_t i = 0; i < lae_array_count( array ); i++ ) {
            fprintf( stdout, "%s\n", lae_array_at( array, i ) );
        }
    }
    
    {
        lae_map * map = lae_map_make( lae_allocator_default(), (lae_map_less)string_less );

        lae_map_set( map, "one", "1" );
        lae_map_set( map, "two", "2" );
        lae_map_set( map, "three", "3" );
        lae_map_set( map, "four", "4" );
        
        lae_map_unset( map, "two" );

        fprintf( stdout, "%s\n", lae_map_get( map, "one" ) );
        fprintf( stdout, "%s\n", lae_map_get( map, "two" ) );
        fprintf( stdout, "%s\n", lae_map_get( map, "three" ) );
        fprintf( stdout, "%s\n", lae_map_get( map, "four" ) );
    }
    
    {
        lae_string * filename = lae_utf8( argv[1] );
        lae_stream * stream = lae_filestream_make( lae_allocator_default(), filename, lae_filestream_access_readonly );
        char buffer[255];
        const lae_stream_result r = lae_stream_read( stream, buffer, sizeof( buffer ), 1 );
        if ( r.error == lae_stream_code_ok ) {
            buffer[r.value] = 0;
            lae_stream_write( lae_stdout(), buffer, 1, r.value );
        } else if ( r.error == lae_stream_code_eof ) {
            buffer[r.value] = 0;
            lae_stream_write( lae_stdout(), buffer, 1, r.value );
        } else {
            lae_stream_write( lae_stderr(), "err", 1, 3 );
        }
        lae_stream_close( stream );
    }
    
    lae_pool_release( pool );
    return 0;
}
