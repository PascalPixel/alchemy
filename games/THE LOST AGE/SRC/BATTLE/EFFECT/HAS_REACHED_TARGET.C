#include "TYPES.H"
#include "EFFECT_SLOT.H"

/* Whether an object that stops at its target still has one to reach. */
u32 BattleFx_HasReachedTarget(struct EffectSlot *effect)
{
    u32 value;

    if (effect->stop_at_target == 0) {
        return 0;
    }
    value = (u32)effect->target_x ^ 0x80000000;
    return ((0u - value) | value) >> 31;
}
