#include "types.h"

#define SceneActor_SetActor10Byte23To1 Func_02000c88

extern u8 *Func_02004826(s32);

void SceneActor_SetActor10Byte23To1(void)
{
    Func_02004826(10)[0x23] = 1;
}
