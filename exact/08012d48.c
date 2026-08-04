#include "types.h"

extern u8 *Data_03001e60;

void Func_08012d48(s32 slot, s32 value)
{
    u8 *base = Data_03001e60;
    s32 offset = (slot & 3) * 4 + 40;
    s32 count = 9;

    do {
        u8 *entry = *(u8 **)(base + offset);

        count--;
        entry[6] = value;
        base += 56;
    } while (count >= 0);
}
