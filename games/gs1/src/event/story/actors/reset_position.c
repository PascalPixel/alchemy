#include "types.h"

extern void Func_02004560(u8 *, s32);
extern void Func_02004580(u8 *, s32);
extern s32 Func_020045c0(s32);
extern void Func_020045d2(s32);
extern u8 Value_000002f1;

s32 StoryActor_ResetPosition(u8 *actor)
{
    s32 zero;
    Func_02004560(actor, 0);
    Func_02004580(actor, 10);
    {
        u8 *mode_flags = actor + 0x59;
        zero = 0;
        *mode_flags = zero;
    }
    if (Func_020045c0(0x8A0) != 0) {
        Func_020045d2((s32)&Value_000002f1);
        *(s32 *)(actor + 8) = zero;
        *(s32 *)(actor + 12) = zero;
    }
    return 0;
}
