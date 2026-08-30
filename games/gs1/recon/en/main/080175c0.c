#include "global_cells.h"
#include "types.h"

s32 Func_08018038(s32 value, s32 mode);
s32 Func_080165d8(s32 no, s32 entry, s32, s32, s32, s32);

s32 Func_080175c0(s32 no, s32 argument)
{
    u8 *base = *(u8 **)ADDR_03001E8C;
    s32 entry;
    s32 entry_offset;
    s32 result = 0;

    *(u16 *)(base + 0x12f4) = 0;
    *(u16 *)(base + 0x12f6) = 0;
    entry = Func_08018038(argument, 1);
    entry_offset = entry * 2;
    entry_offset += 0xeb0;
    if (*(u16 *)(base + entry_offset) == 0)
        return 0;
    if (no == 0)
        return 0;
    result = Func_080165d8(no, entry, 0, 0, 0, 1);
    if (result == 0)
        return 0;
    return result;
}
