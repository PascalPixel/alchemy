#include "TYPES.H"
#include "EFFECT_0809B11C.H"

/* Whether a slot that stops at its target still has one to reach: ☀️'s
   test. The Spanish and Italian editions call this place through their own
   far stub, so it stays a file of its own beside SLOT_SET_POSITION.C. */
u32 BattleFx_HasReachedTarget(struct EffectSlot *effect)
{
    u32 value;

    if (effect->stop_at_target == 0) {
        return 0;
    }
    value = (u32)effect->target_x ^ 0x80000000;
    return ((0u - value) | value) >> 31;
}
