#include "types.h"
#undef Inn_CalculateRoomPrice
#define Inn_CalculateRoomPrice Func_080cd52c


extern u8 *Data_03001eec;

s32 Func_080d6888(s32 value, s32 a, s32 b, s32 c, s32 d);

void Inn_CalculateRoomPrice(void)
{
    u8 *base;
    s32 i;
    s32 index;
    u8 remaining;

    base = Data_03001eec;
    i = 0;
    index = 36;
    do {
        remaining = base[0x7818 + i];
        if (remaining != 0) {
            remaining -= 1;
            if ((base[0x7818 + i] = remaining) == 0) {
                Func_080d6888(
                    *(s16 *)(*(u8 **)(base + 0x7828) + index),
                    0, -1, -1, 0);
            }
        }
        i += 1;
        index += 2;
    } while (i != 8);
}
