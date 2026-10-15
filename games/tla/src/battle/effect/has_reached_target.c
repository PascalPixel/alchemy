#include "types.h"
#include "effect_slot.h"

u32 BattleFx_HasReachedTarget(struct EffectSlot *effect)
{
    u32 value;

    if (effect->stopAtTarget == 0)
    {
        return 0;
    }
    value = (u32)effect->targetX ^ 0x80000000;
    return ((0u - value) | value) >> 31;
}
