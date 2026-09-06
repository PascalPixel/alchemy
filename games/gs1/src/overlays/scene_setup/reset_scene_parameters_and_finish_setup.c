#include "types.h"
/* resource_3c5 owner at 0x02001158, 42 bytes. */
void Func_0200408a();
void Func_0200402c();
void Func_020040d0();
void Func_020040dc();

static __inline__ void ResetSceneParameters(s32 a, s32 b, s32 c, s32 mode)
{
    Func_0200408a(a, b, c, mode);
}

void ResetSceneParametersAndFinishSetup(void)
{
    ResetSceneParameters(-1, -1, -1, 0);
    Func_0200402c(0, 6, 0);
    Func_020040d0();
    Func_020040dc();
}
