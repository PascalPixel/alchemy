#include "types.h"

u8 *Func_08077008(s32);

s32 Func_080b27b0(s32 index, s32 kind)
{
    u8 *entry = Func_08077008(index);
    s32 result = 0;

    if ((kind == 0 && *(s16 *)(entry + 56) <= 0)
        || (kind == 1 && *(s8 *)(entry + 305) != 0)
        || (kind == 2 && entry[320] != 0)
        || (kind == 3 && *(s8 *)(entry + 304) != 0)) {
        result = 1;
    }
    return result;
}
