#include "types.h"

#define SceneState_ConfigureRegion1_0_21x14 Func_02000148

extern void Func_02000a8e(s32, s32, s32, s32, s32, s32);

void SceneState_ConfigureRegion1_0_21x14(void)
{
    s32 w = 21;
    s32 h = 14;

    Func_02000a8e(1, 0, 1, 1, w, h);
}
