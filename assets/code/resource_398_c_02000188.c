#include "types.h"

extern void Func_02000ac6(s32, s32, s32, s32, s32, s32);
extern void Func_02000ae2(s32, s32, s32, s32, s32, s32);

void Func_02000188(void)
{
    {
        s32 fifth = 1;
        s32 sixth = 3;

        Func_02000ac6(111, 37, 97, 21, fifth, sixth);
    }
    {
        s32 fifth = 32;
        s32 sixth = 24;

        Func_02000ae2(46, 38, 3, 2, fifth, sixth);
    }
}
