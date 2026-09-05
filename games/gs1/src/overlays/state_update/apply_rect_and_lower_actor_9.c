#include "types.h"

extern s32 *Func_0200414c(s32);
extern void Func_020040f2(s32, s32, s32, s32, s32, s32);
extern void Func_02004106(s32 *, s32);
extern void Func_02004146(s32);

void SceneState_ApplyRectAndLowerActor9(void)
{
    s32 *rec;
    s32 s0;
    s32 s1;

    rec = Func_0200414c(9);
    s0 = 43;
    s1 = 41;
    Func_020040f2(45, 41, 1, 1, s0, s1);
    if (rec != 0) {
        Func_02004106(rec, 0);
        rec[3] += 0xffe00000;
        ((u8 *)rec)[0x23] = 2;
    }
    Func_02004146(0x200);
}
