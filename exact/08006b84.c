#include "types.h"

void Func_08006b84(u32 src, u8 *dst, s32 size)
{
    const u8 *srcPos = (const u8 *)src;
    u8 *dstPos = dst;
    s32 count = size - 1;

    if (size != 0) {
        do {
            src = *srcPos;
            *dstPos = src;
            srcPos++;
            dstPos++;
            count--;
        } while (count != -1);
    }
}
