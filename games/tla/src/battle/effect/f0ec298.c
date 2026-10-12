#include "types.h"

extern s16 Data_0202a642;

u32 Func_080ec298(u8 *entry)
{
    if (Data_0202a642 != 0)
        return entry[1] >> 4;
    return entry[1] & 15;
}
