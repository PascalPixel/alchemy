#include "types.h"

void Func_0801e9d4(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4);

void Func_08020150(s32 arg0, s32 arg1)
{
    s32 offset;
    s32 remaining;
    volatile u8 *src;
    s32 size;

    if (arg0 != 0) {
        offset = 0;
        size = 0x10;
        remaining = 3;
        src = (volatile u8 *) (arg1 + 0x28);
        do {
            Func_0801e9d4((s8) *src, 2, arg0, offset, size);
            src += 1;
            remaining -= 1;
            offset = offset + 0x18;
        } while (remaining >= 0);
    }
}
