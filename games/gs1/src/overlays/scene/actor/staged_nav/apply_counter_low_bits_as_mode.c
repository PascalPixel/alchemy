/* Apply the actor's low four counter bits as its animation mode. */
#include "types.h"
#include "scene.h"

s32 Actor_ApplyCounterLowBitsAsMode(u8 *actor)
{
    Actor_Apply(actor, *(u16 *)(actor + 100) & 15);
    return 0;
}
