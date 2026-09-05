#include "types.h"
#undef ObjectGroup_TickMemberTimers
#define ObjectGroup_TickMemberTimers Func_080cd52c

extern u8 *Data_03001eec;

s32 Func_080d6888(s32 value, s32 a, s32 b, s32 c, s32 d);

void ObjectGroup_TickMemberTimers(void)
{
    u8 *base;
    s32 i;
    s32 index;

    base = Data_03001eec;
    i = 0;
    do {
        if (base[0x7818 + i] != 0) {
            if ((base[0x7818 + i] = base[0x7818 + i] - 1) == 0) {
                index = i * 2 + 36;
                Func_080d6888(
                    *(s16 *)(*(u8 **)(base + 0x7828) + index),
                    0, -1, -1, 0);
            }
        }
        i += 1;
    } while (i != 8);
}
