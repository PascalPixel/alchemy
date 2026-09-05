#include "types.h"

#define SceneState_ApplyThreeRectsRows9And10 Func_02000410

extern void Func_020014ee(s32, s32, s32, s32, s32, s32);
extern void Func_02001502(s32, s32, s32, s32, s32, s32);
extern void Func_0200151c(s32, s32, s32, s32, s32, s32);

void SceneState_ApplyThreeRectsRows9And10(void)
{
    s32 strip = 17;

    {
        s32 p5 = 80;
        s32 p6 = 9;

        Func_020014ee(90, 9, 2, 3, p5, p6);
    }
    Func_02001502(27, 10, 1, 2, strip, 10);
    Func_0200151c(17, 10, 1, 1, strip, 11);
}
