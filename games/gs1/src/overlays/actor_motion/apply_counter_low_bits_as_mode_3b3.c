/* Apply the actor's low four counter bits as its animation mode. */
#include "types.h"

#define SceneActor_ApplyCounterLowBitsAsMode Func_02001428

void Func_02003fa2(u8 *actor, s32 mode);

s32 SceneActor_ApplyCounterLowBitsAsMode(u8 *actor)
{
    Func_02003fa2(actor, *(u16 *)(actor + 100) & 15);
    return 0;
}
