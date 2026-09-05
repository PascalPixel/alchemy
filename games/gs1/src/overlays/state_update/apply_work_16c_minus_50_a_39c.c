#include "types.h"

#define SceneState_ApplyWork16cMinus50A Func_02001060

extern u8 *Data_03001ebc;
extern void Func_02001ff8(s32);

void SceneState_ApplyWork16cMinus50A(void)
{
    Func_02001ff8(*(s16 *)(Data_03001ebc + 0x16c) - 50);
}
