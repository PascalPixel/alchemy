#include "types.h"

#define SceneState_SetHalfwordB030 Func_02001c78

void SceneState_SetHalfwordB030(u16 value)
{
    *(u16 *)0x0200b030 = value;
}
