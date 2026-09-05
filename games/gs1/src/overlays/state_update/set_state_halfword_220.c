#include "types.h"

#define SceneState_SetStateHalfword220 Func_020038a0

extern u8 *Data_03001f3c;

void SceneState_SetStateHalfword220(s32 a)
{
    u8 *p = Data_03001f3c;

    *(s16 *)(p + 220) = a;
}
