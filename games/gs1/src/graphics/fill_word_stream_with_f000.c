#include "types.h"
#include "scene.h"
#include "abi/graphics/fill_word_stream_with_f000.h"

extern u16 *gIw;

void Graphics_FillWordStreamWithF000(u32 count)
{
    u16 *dst = gIw;
    u32 index;

    for (index = 0; index < count; index++)
        *dst++ = 0xf000;
    gIw = dst;
}
