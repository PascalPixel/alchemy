#include "ov69f.h"

void Func_020000b4(s32 mode)
{
    if (mode == 1) {
        Func_02001bec(133);
        Ov69f_MapPlay(84, 36, 2, 3, 78, 40);
        WaitFrames(6);
    }
    Ov69f_MapPlay(84, 39, 2, 3, 78, 40);
    Ov69f_BlendStart(20, 38, 2, 1, 14, 42);
}

void Func_02000108(void)
{
    Func_02001bec(134);
    Ov69f_MapPlay(84, 36, 2, 3, 78, 40);
    WaitFrames(6);
    Ov69f_MapPlay(84, 42, 2, 3, 78, 40);
    Ov69f_BlendStart(20, 44, 2, 1, 14, 42);
}

void Func_02000154(void)
{
    struct Ov69fObject *object = Object_GetById(12);

    if (!Ov69f_FlagChk(0xa19) && object->x >> 20 == 4 && object->z >> 20 == 37) {
        Ov69f_FlagSet(0xa19);
        Func_020000b4(1);
    }
}
