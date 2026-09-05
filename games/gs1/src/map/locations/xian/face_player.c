#include "types.h"
#define FIELD(base, type, offset) (*(type *)((u8 *)(base) + (offset)))

void *Func_02004730(s32);
s16 Func_020046a2(s32, s32);

s32 FaceXianActorToPlayer(void *actor)
{
    void *player = Func_02004730(0);
    FIELD(actor, u16, 6) = Func_020046a2(FIELD(player, s32, 0x10) - FIELD(actor, s32, 0x10), FIELD(player, s32, 8) - FIELD(actor, s32, 8));
    return 0;
}
