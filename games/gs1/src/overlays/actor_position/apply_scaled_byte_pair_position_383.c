#include "types.h"

#define SceneActor_ApplyScaledBytePairPosition Func_020048a4

extern void Func_0200958c(s32, s32, s32, s32);

void SceneActor_ApplyScaledBytePairPosition(s32 a, u8 *p)
{
    Func_0200958c(a, (p[0] << 19) + 0x480000, 0, (p[1] << 19) + 0x2780000);
}
