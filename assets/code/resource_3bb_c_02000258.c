#include "types.h"

extern void Func_02004150(s32);
extern u8 *Func_020041b6(s32);
extern void Func_02004108(s32, s32, s32, s32, s32, s32);

void Func_02000258(void)
{
    u8 *slot;

    Func_02004150(0x331);
    slot = Func_020041b6(20) + 85;
    *slot = 0;
    {
        s32 fifth = 44;
        s32 sixth = 17;

        Func_02004108(46, 17, 1, 1, fifth, sixth);
    }
}
