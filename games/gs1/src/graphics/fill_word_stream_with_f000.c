#include "types.h"
#include "scene.h"

extern u16 *gIw;

void Graphics_FillWordStreamWithF000(u32 count)
{
    u16 *dst = gIw;
    u32 index;

    for (index = 0; index < count; index++)
        *dst++ = 0xf000;
    gIw = dst;
}
