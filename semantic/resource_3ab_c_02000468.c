#include "types.h"

extern u8 * Func_02001e6e(s32);
extern u8 * Func_02001e7e(s32);
extern u8 * Func_02001e8e(s32);
void Func_02000468(void)
{
    u8 *leader = Func_02001e6e(0);
    if (*(s32 *)(leader + 12) >= 0x100000)
        Func_02001e7e(8)[0x23] |= 2;
    else
        Func_02001e8e(8)[0x23] = 1;
}
