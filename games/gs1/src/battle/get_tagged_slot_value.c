#include "types.h"
#include "global_cells.h"

#define Battle_GetTaggedSlotValue Func_080b9a44

s16 Battle_GetTaggedSlotValue(s32 arg0) {
    u8 *base = *(u8 **)ADDR_03001E74;
    s32 offset;

    if ((arg0 & 0x80) != 0) {
        offset = (arg0 & 0xF) * 2 + 0x64;
        base += 2;
    } else {
        offset = (arg0 & 0xF) * 2 + 0x58;
    }
    return *(s16 *)(base + offset);
}
