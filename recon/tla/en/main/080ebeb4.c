#include "types.h"
#include "effect_slot.h"

void EffectSlot_SetCallback(struct EffectSlot *effect, EffectCallback callback)
{
    effect->callback = callback;
    effect->callback_delay = 0;
    effect->age = 0;
    effect->state = 0;
}
