#include "types.h"

extern u8 *Func_02004812(s32);

void SceneActor_SetActor10Byte23To3(void)
{
    Func_02004812(10)[0x23] = 3;
}
