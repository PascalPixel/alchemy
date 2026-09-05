#include "types.h"

#define SceneState_ApplyRectAndSend303 Func_02000248

extern void Func_02003e56(s32, s32, s32, s32, s32, s32);
extern void Func_02003ec4(s32);

void SceneState_ApplyRectAndSend303(void)
{
    {
        s32 x = 47;
        s32 y = 12;

        Func_02003e56(47, 24, 1, 1, x, y);
    }
    Func_02003ec4(0x303);
}
