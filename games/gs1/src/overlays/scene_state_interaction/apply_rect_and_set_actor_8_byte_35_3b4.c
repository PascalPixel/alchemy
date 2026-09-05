#include "types.h"

extern s32 Func_02003f5e(s32, s32, s32, s32, s32, s32);
extern u8 *Func_02003fcc(s32);

void SceneState_ApplyRectAndSetActor8Byte35(void)
{
    s32 w = 41;
    s32 h = 42;
    u8 *p;

    Func_02003f5e(44, 42, 1, 1, w, h);
    p = Func_02003fcc(8) + 35;
    *p = 2;
}
