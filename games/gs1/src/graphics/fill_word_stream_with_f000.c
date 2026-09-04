#include "types.h"

#define Graphics_FillWordStreamWithF000 Func_08004698

extern u16 *Data_03001cbc;

void Graphics_FillWordStreamWithF000(u32 count)
{
    u16 *dst = Data_03001cbc;
    u32 index;

    for (index = 0; index < count; index++)
        *dst++ = 0xf000;
    Data_03001cbc = dst;
}
