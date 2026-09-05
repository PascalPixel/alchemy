#include "types.h"

#define SceneState_ClearRecordZeroBit0At35 Func_020015f8

extern s32 Func_0200521a(s32);

void SceneState_ClearRecordZeroBit0At35(void) {
    u8 *p;
    s32 v;

    p = (u8 *)(Func_0200521a(0) + 35);
    v = 254;
    v &= *p;
    *p = v;
}
