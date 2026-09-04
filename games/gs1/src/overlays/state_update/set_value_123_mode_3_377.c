#include "types.h"

#define SceneState_SetValue123Mode3 Func_02000344

extern void Func_02001df6(s32);
extern void Func_02000630(s32);

void SceneState_SetValue123Mode3(void)
{
    Func_02001df6(0x7B);
    Func_02000630(3);
}
