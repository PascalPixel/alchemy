#include "ov6a0.h"

void Func_02000060(s32 mode)
{
    if (mode == 1) {
        Func_02002c5c(133);
        Call6((void (*)())Func_02002b2c, 76, 42, 4, 3, 76, 35);
        WaitFrames(6);
    }
    Func_02002b2c(81, 42, 4, 3, 76, 35);
    Func_02002b24(12, 41, 4, 1, 12, 35);
}

void Func_020000b4(void)
{
    Func_02002c5c(134);
    Func_02002b2c(76, 42, 4, 3, 76, 35);
    WaitFrames(6);
    Func_02002b2c(71, 42, 4, 3, 76, 35);
    Func_02002b24(12, 36, 4, 1, 12, 35);
}

void Func_02000100(void)
{
    struct ObjectRuntime *object = Object_GetById(16);

    if (!Value1((s32 (*)())GameFlag_Test, 0xa1b) && object->x >> 20 == 8 && object->z >> 20 == 37) {
        Call1((void (*)())GameFlag_SetBit, 0xa1b);
        Func_02000060(1);
    }
}
