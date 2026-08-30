#include "global_cells.h"
#include "types.h"

s32 Func_08018038(s32 value, s32 mode);
s32 Func_080165d8(s32 no, s32 entry, s32, s32, s32, s32);

s32 Func_080175c0(s32 no, s32 argument, s32 unused2, s32 unused3, s32 unused4)
{
    u8 *base = *(u8 **)ADDR_03001E8C;
    s32 entry;
    s32 result = 0;

    *(u16 *)(base + 0x12f4) = 0;
    *(u16 *)(base + 0x12f6) = 0;
    entry = Func_08018038(argument, 1);
    if (*(u16 *)(base + 0xeb0 + entry * 2) != 0 && no != 0)
        result = Func_080165d8(no, entry, 0, 0, 0, 1);
    if (result == 0)
        return 0;
    return result;
}
