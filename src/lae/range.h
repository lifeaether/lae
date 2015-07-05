#ifndef __lae__range__
#define __lae__range__

#include <stdint.h>

typedef struct {
    int32_t     location;
    uint32_t    length;
} lae_range;

lae_range lae_range_make( const int32_t location, const uint32_t length );
int32_t lae_range_begin ( const lae_range range );
int32_t lae_range_end   ( const lae_range range );

#endif
