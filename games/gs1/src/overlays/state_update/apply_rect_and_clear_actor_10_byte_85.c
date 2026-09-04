#include "types.h"

#define SceneState_ApplyRectAndClearActor10Byte85 Func_02001ccc

extern s32 Func_020041ba(s32, s32, s32, s32, s32, s32);
extern u8 *Func_02004228(s32);

void SceneState_ApplyRectAndClearActor10Byte85(void)
{
    s32 w = 42;
    s32 h = 55;
    u8 *p;

    Func_020041ba(40, 54, 1, 1, w, h);
    p = Func_02004228(10) + 85;
    *p = 0;
}
