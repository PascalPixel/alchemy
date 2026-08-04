#include "types.h"

extern void Func_020024ee(s32, s32, s32, s32, s32, s32);
extern void Func_0200250a(s32, s32, s32, s32, s32, s32);
extern void Func_02002540(s32);

void Func_02001028(void)
{
    {
        s32 fifth = 1;
        s32 sixth = 2;

        Func_020024ee(6, 28, 5, 13, fifth, sixth);
    }
    {
        s32 fifth = 5;
        s32 sixth = 13;

        Func_0200250a(6, 28, 1, 2, fifth, sixth);
    }
    Func_02002540(1);
}
