#include "types.h"

extern u16 *Data_03001cbc;

void Graphics_FillWordStreamWithF000(u32 count)
{
    u16 *dst = Data_03001cbc;
    u32 index;

    for (index = 0; index < count; index++)
        *dst++ = 0xf000;
    Data_03001cbc = dst;
}
