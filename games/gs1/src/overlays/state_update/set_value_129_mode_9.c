#include "types.h"

#define SceneState_SetValue129Mode9 Func_020003bc

extern void Func_02001e6e(s32);
extern void Func_020006a8(s32);

void SceneState_SetValue129Mode9(void)
{
    Func_02001e6e(0x81);
    Func_020006a8(9);
}
