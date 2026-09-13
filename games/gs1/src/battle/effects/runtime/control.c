#include "effect_0809b11c.h"

u32 EffectSlot_HasReachedTarget(struct EffectSlot *effect)
{
    u32 value;

    if (effect->flag41 == 0) {
        return 0;
    }
    value = (u32)effect->target_x ^ 0x80000000;
    return ((0u - value) | value) >> 31;
}


void EffectSlot_SetPosition(struct EffectSlot *effect, s32 x, s32 z)
{
    effect->target_x = EFFECT_NO_TARGET;
    effect->target_z = EFFECT_NO_TARGET;
    effect->x = x;
    effect->z = z;
    effect->speed = 0;
}


void Object_InitializeMode(void *, s32);

void EffectSlot_SetObjectMode(struct EffectSlot *effect, s32 mode)
{
    Object_InitializeMode(effect->object, mode);
}


void EffectSlot_SetCallback(struct EffectSlot *effect, EffectCallback callback)
{
    effect->callback = callback;
    effect->callback_delay = 0;
    effect->age = 0;
    effect->state = 0;
}

