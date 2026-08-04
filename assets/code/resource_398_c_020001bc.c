#include "types.h"

extern void Func_02000afa(s32, s32, s32, s32, s32, s32);
extern void Func_02000b16(s32, s32, s32, s32, s32, s32);

void Func_020001bc(void)
{
    {
        s32 fifth = 1;
        s32 sixth = 3;

        Func_02000afa(95, 21, 97, 21, fifth, sixth);
    }
    {
        s32 fifth = 32;
        s32 sixth = 25;

        Func_02000b16(46, 38, 3, 1, fifth, sixth);
    }
}
