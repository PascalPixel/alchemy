#include "types.h"

extern u8 Data_03001b10[];
s32 Func_08003f04(s32 entry_no);

s32 Func_08003f78(u32 entry_no)
{
    u16 *entry = (u16 *)(Data_03001b10 + entry_no * 4);

    if (entry_no > 95)
        return -1;
    if (*entry > 16) {
        s32 value;

        Func_08003f04(entry_no);
        value = 1;
        *entry = value;
    }
    return 0;
}
