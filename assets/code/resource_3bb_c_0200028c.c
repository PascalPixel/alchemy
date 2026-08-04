#include "types.h"

extern void Func_02004184(s32);
extern u8 *Func_020041ea(s32);
extern void Func_0200413c(s32, s32, s32, s32, s32, s32);

void Func_0200028c(void)
{
    u8 *slot;

    Func_02004184(0x332);
    slot = Func_020041ea(21) + 85;
    *slot = 0;
    {
        s32 fifth = 50;
        s32 sixth = 17;

        Func_0200413c(46, 17, 1, 1, fifth, sixth);
    }
}
