#include "types.h"

s32 Func_080796c4(s16 *output);
s32 Func_08078af8(s32 arg0, s32 value);

s32 Func_08078b60(s32 value)
{
    u16 owners[16];
    s32 result = 0;
    s32 count = Func_080796c4(owners);

    if (result < count) {
        u16 *owner = owners;
        s32 remaining = count;

        do {
            result += Func_08078af8(*owner++, value);
            remaining--;
        } while (remaining != 0);
    }
    return result;
}
