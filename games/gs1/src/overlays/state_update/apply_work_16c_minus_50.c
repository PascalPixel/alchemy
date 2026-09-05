#include "types.h"

#define SceneState_ApplyWork16cMinus50 Func_02001080

extern u8 *Data_03001ebc;
extern void Func_02002018(s32);

void SceneState_ApplyWork16cMinus50(void)
{
    Func_02002018(*(s16 *)(Data_03001ebc + 0x16c) - 50);
}
