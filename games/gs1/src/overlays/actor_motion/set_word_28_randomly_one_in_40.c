#include "types.h"

#define SceneActor_SetWord28RandomlyOneIn40 Func_0200089c

extern s32 Func_02004b0e(void);

s32 SceneActor_SetWord28RandomlyOneIn40(u8 *obj)
{
    if ((((u32)(Func_02004b0e() * 40)) >> 16) == 0)
        *(s32 *)(obj + 0x28) = 0x40000;
    return 1;
}
