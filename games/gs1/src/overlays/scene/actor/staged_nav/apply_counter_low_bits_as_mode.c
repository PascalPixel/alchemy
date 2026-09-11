/* Apply the actor's low four counter bits as its animation mode. */
#include "types.h"
#include "scene.h"
#include "abi/overlays/scene/actor/staged_nav/apply_counter_low_bits_as_mode.h"

s32 Actor_ApplyCounterLowBitsAsMode(u8 *actor)
{
    Actor_Apply(actor, *(u16 *)(actor + 100) & 15);
    return 0;
}
