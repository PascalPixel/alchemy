#include "types.h"

u8 Func_080793f8(s32 no)
{
    u8 *base;

    base = (u8 *)0x02000040;
    no = ((u32)no << 0x14) >> 0x17;
    if (base[no] != 0) {
        base[no] = base[no] + 0xFF;
    }
    return base[no];
}
