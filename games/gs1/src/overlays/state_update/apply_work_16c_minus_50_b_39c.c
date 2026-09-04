#include "types.h"

#define SceneState_ApplyWork16cMinus50B Func_020010a0

extern u8 *Data_03001ebc;
extern void Func_02002038(s32);

void SceneState_ApplyWork16cMinus50B(void)
{
    Func_02002038(*(s16 *)(Data_03001ebc + 0x16c) - 50);
}
