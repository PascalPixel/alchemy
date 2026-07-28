#include "types.h"

#define M2C_FIELD(base, type, offset)     (*(type *)((u8 *)(base) + (offset)))

extern u8 Data_03001810[];

s32 Func_08003f04(s32 entry_no)
{
    s32 count = 0;
    s32 i;
    u8 *p;
    u8 fill;
    if ((u32)entry_no > 0x5F) {
        return -1;
    }
    p = Data_03001810;
    fill = 0xFF;
    i = 0x200;
    do {
        if (*p == entry_no) {
            *p = fill;
            count++;
        }
        i--;
        p++;
    } while (i != 0);
    if (count != 0) {
        return -1;
    }
    return 0;
}
