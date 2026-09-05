#include "types.h"

#define SceneState_SetValueEe4 Func_020045fc

extern void Func_02008cfc(void);
extern void Func_02008cdc(s32, s32);
extern void Func_02008d10(void);

void SceneState_SetValueEe4(void)
{
    Func_02008cfc();
    Func_02008cdc(0xee4, 1);
    Func_02008d10();
}
