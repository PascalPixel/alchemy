#include "types.h"

u8 *Func_08077008(void);

s32 Func_080b2778(s32 unused, s32 kind)
{
    u8 value = Func_08077008()[0xF];
    s32 result = 0;

    if (kind == 0) {
        result = value * 20;
    } else if (kind == 1) {
        result = 10;
    } else if (kind == 2) {
        result = 50;
    } else if (kind == 3) {
        result = value * 10;
    }
    return result;
}
