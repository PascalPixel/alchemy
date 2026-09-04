#include "types.h"

#define SceneState_PassWorkHalfword16C Func_02000964

extern u8 *Data_03001ebc;

extern void Func_020017aa(s32);

void SceneState_PassWorkHalfword16C(void)
{
    s16 *cnt = (s16 *)(Data_03001ebc + 0x16C);

    Func_020017aa(*cnt);
}
