#include "types.h"

s16 *Func_08077018(s32);

s32 Func_080b19cc(s32 arg0)
{
    s32 val;

    val = *Func_08077018(arg0);

    if (((u8 *)Func_08077018(arg0))[3] & 8) {
        val = 0;
    } else if (arg0 & 0x400) {
        val = val / 2;
    } else {
        val = val * 3 / 4;
    }
    return val;
}
