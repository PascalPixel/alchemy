#include "fixed_math.h"
#include "types.h"

u8 *Func_08077008(s32);
s32 Func_080b6b40(s32 arg0, u16 *arg1);
u32 Random16(void);
/* LCG: seed = seed * 0x41c64e6d + 0x3039, returns bits 8-23. */
#define Rand Random16

s32 Func_080b8f08(const u8 *arg0)
{
    s16 values[14];
    s32 index;
    s32 count;

    index = *(s16 *)(arg0 + 0xA);
    if (*(s16 *)(Func_08077008(index) + 0x38) != 0) {
        return index;
    }

    if (index > 0x7F) {
        count = Func_080b6b40(2, values);
    } else {
        count = Func_080b6b40(1, values);
    }

    if (count == 0) {
        return 0x100;
    }

    return values[(u32)(Rand() * count) >> 0x10];
}
