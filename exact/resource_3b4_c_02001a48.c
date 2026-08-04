#include "types.h"

extern s32 Func_02003f34(s32, s32, s32, s32, s32, s32);
extern u8 *Func_02003fa2(s32);

void Func_02001a48(void)
{
    s32 extent = 42;
    u8 *entry;

    Func_02003f34(58, 41, 1, 1, extent, extent);
    entry = Func_02003fa2(8) + 35;
    *entry = 2;
}
