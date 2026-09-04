#include "types.h"

#define SceneState_ApplyTwoRects Func_02000188

extern void Func_02000ac6(s32, s32, s32, s32, s32, s32);
extern void Func_02000ae2(s32, s32, s32, s32, s32, s32);

void SceneState_ApplyTwoRects(void)
{
    {
        s32 a5 = 1;
        s32 a6 = 3;

        Func_02000ac6(111, 37, 97, 21, a5, a6);
    }
    {
        s32 a5 = 32;
        s32 a6 = 24;

        Func_02000ae2(46, 38, 3, 2, a5, a6);
    }
}
