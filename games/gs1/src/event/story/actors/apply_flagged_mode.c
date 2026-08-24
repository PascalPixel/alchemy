#include "types.h"

extern void Func_02000576(void);
extern s32 Func_0200453c(s32);
extern void Func_020044b8(u8 *, s32);
extern u8 Value_00000847;

s32 StoryActor_ApplyFlaggedMode(u8 *actor)
{
    Func_02000576();
    if (Func_0200453c((s32)&Value_00000847) != 0) {
        Func_020044b8(actor, 2);
    }
    return 1;
}
