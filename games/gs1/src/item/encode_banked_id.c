#include "types.h"

u8 *Item_GetData(u32);

u32 Item_EncodeBankedId(u32 value)
{
    u32 bank = 0;
    u32 base = value & 0x1ff;
    if (base == 0)
        return 0;
    {
        u8 flags = Item_GetData(base)[3];

        if (flags & 8)
            bank = 1;
        bank <<= 1;
        if (flags & 4)
            bank++;
        bank <<= 9;
        bank += base;
    }
    return bank;
}
