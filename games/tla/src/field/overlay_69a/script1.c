#include "types.h"

void Func_02001bd4(s32 a0);
void Func_02001aec(s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5);
void WaitFrames(s32 a0);
void Func_02001ae4(s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5);

void Func_020000ec(void)
{
    Func_02001bd4(134);
    Func_02001aec(81, 20, 2, 3, 79, 29);
    Func_02001aec(81, 20, 2, 3, 79, 37);
    WaitFrames(6);
    Func_02001aec(87, 20, 2, 3, 79, 29);
    Func_02001aec(87, 20, 2, 3, 79, 37);
    Func_02001ae4(23, 22, 2, 1, 15, 31);
    Func_02001ae4(23, 22, 2, 1, 15, 39);
}
