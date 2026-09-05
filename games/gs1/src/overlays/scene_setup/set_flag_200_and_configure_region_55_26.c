#include "types.h"

extern void Func_02001df6(s32 flag);
extern void Func_02001dda(s32 kind, s32 x, s32 enabled, s32 mode,
                          s32 a, s32 b);
void SceneState_SetFlag200AndConfigureRegion55_26(void)
{
    Func_02001df6(0x200);
    {
        s32 a = 23;
        s32 b = 26;
        Func_02001dda(55, 26, 4, 2, a, b);
    }
}
