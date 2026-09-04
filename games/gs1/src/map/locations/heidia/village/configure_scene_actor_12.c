#include "types.h"

void Func_0200613a(s32, s32, s32, s32, s32, s32);
void Func_0200614c(s32, s32, s32, s32, s32, s32);
u8 *Func_020061ca(s32);
void Func_0200616e(u8 *, s32);

#define ConfigureSceneActor12 Func_02000b4c
void ConfigureSceneActor12(void)
{
    s32 actor_slot = 15;
    u8 *actor;

    Func_0200613a(15, 20, 1, 1, actor_slot, 22);
    Func_0200614c(17, 23, 1, 3, actor_slot, 23);
    actor = Func_020061ca(12);
    if (actor != 0) {
        Func_0200616e(actor, 0);
        actor[0x55] = 0;
        actor[0x23] = 2;
    }
}
