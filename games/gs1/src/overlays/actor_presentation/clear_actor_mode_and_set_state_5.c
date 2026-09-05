#include "types.h"

extern u8 *Func_02007ebe(s32);
extern void Func_02007e72(u8 *, s32);
extern void Func_02007e0a(u8 *, s32);
extern void Func_02007fa2(s32, s32);

void SceneActor_ClearActorModeAndSetState5(s32 no)
{
    u8 *p;
    s32 mask;

    p = Func_02007ebe(no);
    p[0x55] = 0;
    mask = 252;
    mask &= p[0x59];
    p[0x59] = mask;
    Func_02007e72(p, 0);
    Func_02007e0a(p, 5);
    Func_02007fa2(no, 3);
    {
        s32 v = 2;
        v |= p[0x23];
        p[0x23] = v;
    }
}
