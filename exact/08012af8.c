#include "metadata_lookup.h"
#include "types.h"

s32 Func_08012af8(s32 start, s32 step)
{
    s32 value = start;
    s32 delta = step + value - value;

    for (;;) {
        value += delta;
        if (value < 0) {
            value = 0x200;
            continue;
        }
        {
            s32 limit = 0x200;
            if (value >= limit) {
                value = -1;
                continue;
            }
        }
        if (*(u8 *)Func_08185000(value)) {
            return value;
        }
    }
}
