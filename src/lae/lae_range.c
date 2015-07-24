#include <lae/lae_range.h>

lae_range lae_range_make( const int32_t location, const uint32_t length )
{
    lae_range range = { location, length };
    return range;
}

int32_t lae_range_begin ( const lae_range range )
{
    return range.location;
}

int32_t lae_range_end   ( const lae_range range )
{
    return range.location + range.length;
}
