#include "types.h"
#include "global_cells.h"

extern s32 Func_08009270(s32, s32, s32, s32);

s32 Func_080b6e30(s32 value)
{
    u8 *base;
    s32 offset;
    s32 index;
    s16 item;

    base = *(u8 **)ADDR_03001E74;
    index = 0;
    do {
        offset = index * 2 + 4;
        item = *(s16 *)(offset + (u32)base);
        if (item == value) {
            Func_08009270(index, 0, 0, 0);
            *(s16 *)(offset + (u32)base) = 0;
        }
        index++;
    } while (index <= 5);
}
