#include "types.h"

#define SceneState_ConfigureRegion0_0_21x14 Func_02000168

extern void Func_02000aae(s32, s32, s32, s32, s32, s32);

void SceneState_ConfigureRegion0_0_21x14(void)
{
    s32 w = 21;
    s32 h = 14;

    Func_02000aae(0, 0, 1, 1, w, h);
}
