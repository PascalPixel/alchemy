#include "types.h"

#define M2C_FIELD(base, type, offset)     (*(type *)((u8 *)(base) + (offset)))

void Func_08003dec(s32 *entry, s32 slot)
{
    s32 *previous;
    s32 offset;
    s32 index;

    index = slot;
    if (index > 0xFF) {
        index = 0xFF;
    }
    if (index < 0) {
        index = 0;
    }
    offset = index * 4;
    previous = M2C_FIELD(offset, s32 **, 0x03001400);
    M2C_FIELD(offset, s32 **, 0x03001400) = entry;
    *entry = previous;
}
