#include "types.h"

void Func_02003a08(s32, s32, s32, s32);
s32 Func_020031d6();
void Func_0200383a();

static __inline__ void ConfigureScene(s32 actor, s32 x, s32 y, s32 mode)
{
    Func_02003a08(actor, x, y, mode);
}

#define ConfigureSceneAndCheckActors Func_020015d4

void ConfigureSceneAndCheckActors(void)
{
    ConfigureScene(2, 0x00d00000, 0x00700000, 0);
    if (Func_020031d6(10, 14, 7) != 0) {
        Func_0200383a();
    }
}
