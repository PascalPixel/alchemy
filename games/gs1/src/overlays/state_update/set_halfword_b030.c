#include "types.h"

void SceneState_SetHalfwordB030(u16 value)
{
    *(u16 *)0x0200b030 = value;
}
