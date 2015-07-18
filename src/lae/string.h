#ifndef __lae__string__
#define __lae__string__

#include <lae/allocator.h>

struct lae_string;
typedef struct lae_string lae_string;

typedef enum {
    lae_string_encoding_unknwon = 0,
    lae_string_encoding_ascii   = 1,
    lae_string_econding_utf8    = 10,
    lae_string_econding_utf16   = 11,
    lae_string_econding_utf32   = 12
} lae_string_encodings;

lae_string *    lae_string_make         ( lae_allocator * allocator, const char * bytes, const size_t size, const lae_string_encodings encoding );
lae_string *    lae_string_create       ( lae_allocator * allocator, const char * bytes, const size_t size, const lae_string_encodings encoding );
void            lae_string_release      ( lae_string * string );

size_t                  lae_string_size     ( const lae_string * string );
const char *            lae_string_bytes    ( const lae_string * string );
lae_string_encodings    lae_string_encoding ( const lae_string * string );

size_t                  lae_string_count    ( const lae_string * string );
uint32_t                lae_string_at       ( const lae_string * string, const size_t index );

#endif /* defined(__lae__string__) */
